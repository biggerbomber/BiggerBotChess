import sqlite3
from pwn import process
from tqdm import tqdm
from sklearn.linear_model import SGDRegressor
from sklearn.neural_network import MLPRegressor
import numpy as np
from concurrent.futures import ProcessPoolExecutor
import multiprocessing as mp

ENGINE_PATH = "./src/BiggerBotChess"
NUM_ENGINES = 8              # Adjust based on CPU cores
NUM_POSITIONS = 100000


def create_connection(db_file):
    return sqlite3.connect(db_file)


# ------------ Worker Code (runs inside each process) ------------ #

def init_worker():
    """Each worker creates its own engine instance once."""
    global ENGINE
    ENGINE = process(ENGINE_PATH)
    ENGINE.sendline(b"test")  # warmup handshake


def process_fen(data):
    """Worker: takes (fen, eval) and returns (features, eval)."""
    fen, eval_value = data

    ENGINE.sendline(fen.encode())
    output = ENGINE.recvline().decode().strip().split(",")

    features = [float(x) for x in output]
    return features, float(eval_value)


# ------------ Main Program ------------ #

if __name__ == "__main__":
    # Load DB
    conn = create_connection("scripts/2021-07-31-lichess-evaluations-37MM.db/test.db")
    cursor = conn.cursor()
    cursor.execute("SELECT fen, eval FROM evaluations LIMIT ?", (NUM_POSITIONS,))
    rows = cursor.fetchall()

    # Parallel feature extraction using multiple engines
    with ProcessPoolExecutor(
        max_workers=NUM_ENGINES,
        initializer=init_worker   # each worker gets its own engine
    ) as executor:

        results = list(
            tqdm(executor.map(process_fen, rows), total=len(rows))
        )

    # Unpack results
    X = np.array([feat for feat, _ in results])
    y = np.array([ev for _, ev in results])

    # Train model
    model = MLPRegressor(max_iter=100000, tol=1e-3)
    model.fit(X, y)

    print("Training done.")
    print("Weights:", model.coefs_)
    print("Bias:", model.intercepts_)
    print("Score:", model.score(X, y))

    conn.close()
