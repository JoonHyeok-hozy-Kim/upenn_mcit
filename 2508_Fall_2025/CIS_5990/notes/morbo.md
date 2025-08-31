[Back to Main](../../main.md)

# MORBO code 

## Classes and Functions
  - Trust Region Related
    - `morbo.trust_region.TurboHParams`
    - `morbo.trust_region.TrustRegion`
      - `morbo.trust_region.ScalarizedTrustRegion`
      - `morbo.trust_region.HypervolumeTrustRegion`
  - MOBO management
    - `morbo.state.TabuSet`
    - `morbo.state.TRBOState`
  - Main logic
    - `morbo.run_one_replication.run_one_replication`
  - Evaluation (Benchmark) Function
    - `morbo.benchmark_function.BenchmarkFunction`

## Procedure
#### Initial Run
- `__init__.py` (nothing in it)
- `experiments.main.py`
  - What it does
    - Get arguments
      - `label` is the evaluation function 
        - e.g.) DTLZ
    - Get hyperparameters for the `label` from `experiments`
      - e.g.) dtlz2_10
        ```python
        {
            "dim": 10,
            "evalfn": "DTLZ2",
            "max_evals": 2000,
            "n_initial_points": 200,
            "batch_size": 50,
            "min_tr_size": 200,
            "max_reference_point": [
            -6,
            -6
            ],
            "verbose": true
        }
        ```
    - These are stored in `kwargs`  
    - Call `run_one_replication()` passing `kwargs` 

#### Main algorithm : run_one_replication()
- Determine MOO related hyperparameters
  - `bounds`
    - For predefined Botorch moo functions take its `bounds` member.
        - e.g.) 
          ```python
          from botorch.test_functions.multi_objective import DTLZ2

          problem = DTLZ2
          bounds = problem.bounds
          ```
  - `num_outputs`
  - `num_objectives`
  - `max_reference_point` : from `kwargs`
- Determine the evaluation function `f`.
  - With `kwargs` recognize the evaluation function.
    - e.g.) DTLZ, MW7, rover
  - Instantiate `f`.
    ```python
    f = BenchmarkFunction()
    ```
    - Desc.)
      - `base_f` takes predefined Botorch moo functions
        - e.g.) `from botorch.test_functions.multi_objective import DTLZ2`
- Set failure streak for TR.
- Instantiate `TurboHParams`.
  ```python
  tr_hparams = TurboHParams()
  ```
  - Included parameters (members)
    - TR related
    - MOBO related
      - `hypervolume` : choose the type of TR between `ScalarizedTrustRegion` and `HypervolumeTrustRegion`
      - `winsor_pct` : manages winsorizing in `TrustRegion.update_model()` when fitting the GP model
      - `use_simple_rff` : enables using randomized fourier features
        - Disabled. Not supported for Botorch 0.15
- Instantiate `TRBOState`.
  ```python
  trbo_state = TRBOState()
  ```
  - Members
    - TR related
      - `trust_regions` : a list that contains TRs.
        - Each TR object contains the `model` member
          - Single Obj : `from botorch.models.model import Model`
          - MOO : `from botorch.models.model_list_gp_regression import ModelListGP`
        - These `model`s are created in `TrustRegion.update_model()`.
      - `tr_hparams` : `TurboHParams` object
    - MOBO related
      - `objective`
        - `IdentityMCObjective` for the single objective
        - `IdentityMCMultiOutputObjective` for the multi-objective
      - `pareto_X` : Initialized to `torch.empty((0,d),)`
      - `pareto_Y` : Initialized to `torch.empty((0,m),)`
- Create containers for logging
- Create initial points.
  - `n_points`
  - `X_init`
    - Drawn from sobol samples.
    - Bounds are from `f`.
  - `Y_init`
- `trbo_state.update()`
  - What it does
    - Add `X` and `Y` to `X_history` and `Y_history` respectively.
    - Increment the number of evaluations
      - `self.n_evals.add_(X.shape[0])`
    - If the TR is `HypervolumeTrustRegion`, update `pareto_X`, `pareto_Y`, and `hv`.
      - How?)
        - Add `X` and `Y` to `pareto_X` and `pareto_Y` respectively.
        - Pick pareto dominating points using `from botorch.utils.multi_objective.pareto import is_non_dominated`
        - Calculate HVI using `self.hv_contributions = self.hv - partitioning.compute_hypervolume()`
          - where `partitioning = BoxDecompositionList(*partitionings)`
- `trbo_state.log_restart_points()`
- Iterate the TRs and initialize them.
  - for i-th TR
    - `trbo_state.initialize_standard(tr_idx=i, ...)`
      - `TRBOState.initialize_standard`
      - `TrustRegion.update()`
      - `TrustRegion.update_model()`
      - `morbo.utils.get_fitted_model`
      - gpytorch optimization!
- `trbo_state.update_data_across_trs()` : Needs verification
- `trbo_state.TR_index_history.fill_(-2)`
- while Loop until reaches the `max_evals`
  - Thompson Sampling



[Back to Main](../../main.md)