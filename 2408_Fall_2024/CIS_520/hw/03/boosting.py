def boosting_fit(X, y, T, fit_logistic_clf, predict_logistic_clf):
    # X := Tensor(float) of size (m,d) -- Batch of m examples of demension d
    # y := Tensor(int) of size (m) -- the given vectors of labels of the examples
    # T := Maximum number of models to be implemented

    m = X.size(0)
    clfs = []
    H_prev = torch.zeros((m,))  # Initial ensemble prediction H_1 = 0
    while len(clfs) < T:
        # Calculate the weights for each sample mu. You may need to
        # divide this into the base case and the inductive case.

        ## ANSWER
        if len(clfs) == 0:
            mu = torch.full((m,), 1 / m)  # Initialize weights uniformly
        else:
            prev_alpha, prev_clf = clfs[-1]
            H_prev = (H_prev + prev_alpha * predict_logistic_clf(X, prev_clf)).sign()
            mu = torch.exp(-y * H_prev)
            mu /= mu.sum()  # Normalize weights
        ## END ANSWER

        # Here, we draw samples according to mu and fit a weak classifier
        idx = torch.multinomial(mu, m, replacement=True)
        X0, y0 = X[idx], y[idx]

        clf = fit_logistic_clf(X0, y0)

        # Calculate the epsilon error term

        ## ANSWER
        predictions = predict_logistic_clf(X, clf).sign()
        wrongs = (predictions != y).float()
        eps = (mu * wrongs).sum()  # Weighted error rate
        ## END ANSWER

        if eps > 0.5:
            # In the unlikely even that gradient descent fails to
            # find a good classifier, we'll skip this one and try again
            continue

        # Calculate the alpha term here

        ## ANSWER
        alpha = 0.5 * torch.log((1 - eps) / eps)
        ## END ANSWER

        clfs.append((alpha,clf))
    return clfs

def boosting_predict(X, clfs, predict_logistic_clf):
    # X := Tensor(float) of size (m,d) -- Batch of m examples of demension d
    # clfs := list of tuples of (float, logistic classifier) -- the list of boosted classifiers
    # Return := Tnesor(int) of size (m) -- the predicted labels of the dataset

    # TODO
    pass
