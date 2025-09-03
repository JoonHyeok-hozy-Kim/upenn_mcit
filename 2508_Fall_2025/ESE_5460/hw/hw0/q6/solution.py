import numpy as np
from sklearn.datasets import fetch_california_housing


def solve_linear_model(X, y):
    X_tilde = X - np.mean(X, axis=0)
    y_tilde = y - np.mean(y)

    w = np.linalg.inv(X_tilde.T @ X_tilde) @ (X_tilde.T @ y_tilde)
    b = np.mean(y) - np.mean(X, axis=0) @ w

    return w, b

def get_mse(X, y, w, b):
    y_pred = X @ w + b
    return np.mean((y - y_pred) ** 2)

if __name__ == "__main__":
    # Fetch dataset
    ds = fetch_california_housing()
    # print(ds.keys())
    # print(ds.DESCR)

    X, y = ds.data, ds.target
    print(f"Data shape: {X.shape}, Target shape: {y.shape}")
    n_samples = X.shape[0]

    n_iter = 3    
    seed = 6
    mse_train_list = [None] * n_iter
    mse_test_list = [None] * n_iter

    for i in range(n_iter):
        rand = np.random.default_rng(seed+i)
        indices = rand.permutation(n_samples)
        split = int(0.8 * n_samples)

        # 80% train, 20% test
        train_idx, test_idx = indices[:split], indices[split:]
        mask = np.zeros(n_samples, dtype=bool)
        mask[train_idx] = True

        X_train, y_train = X[mask], y[mask]
        X_test, y_test = X[~mask], y[~mask]

        print(f"Iteration {i+1}")
        print(f"Train samples: {X_train.shape[0]}, Test samples: {X_test.shape[0]}")

        w, b = solve_linear_model(X_train, y_train)
        print(f"weights: {w.ravel()}, bias: {b}")

        mse_train_list[i] = get_mse(X_train, y_train, w, b)
        mse_test_list[i] = get_mse(X_test, y_test, w, b)

        print(f"Train MSE: {mse_train_list[i]}, Test MSE: {mse_test_list[i]}\n")

    print(f"Training error statistics: mean={np.mean(mse_train_list)}, std={np.std(mse_train_list)}")
    print(f"Testing error statistics: mean={np.mean(mse_test_list)}, std={np.std(mse_test_list)}")