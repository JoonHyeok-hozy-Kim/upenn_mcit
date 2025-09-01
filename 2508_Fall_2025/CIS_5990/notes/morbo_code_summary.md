[Back to Main](../../main.md)

# MORBO code 

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
  - Instantiate [BenchmarkFunction](#31-benchmarkfunction): `f = BenchmarkFunction()`
    - Desc.)
      - `base_f` takes predefined Botorch moo functions
        - e.g.) `from botorch.test_functions.multi_objective import DTLZ2`
- Set failure streak for TR.
- Instantiate [TurboHParams](#11-turbohparams): `tr_hparams = TurboHParams()`
- Instantiate [TRBOState](#21-trbostate): `trbo_state = TRBOState()`
- Create containers for logging
- Create initial points.
  - `n_points`
  - `X_init`
    - Drawn from sobol samples
      - `from botorch.utils.sampling import draw_sobol_samples`
    - Bounds are from `f`.
  - `Y_init`
- `trbo_state.update()`
  - Jobs
    - Add new data to history
    - Increment evaluation counts: `n_evals`
    - Update HV if TR is [HypervolumeTrustRegion](#122-hypervolumetrustregion)
- `trbo_state.log_restart_points()`
- Iterate the TRs and initialize them.
  - `for i, tr in enumerate(self.trust_regions)`
    - [trbo_state](#21-trbostate)`.initialize_standard(tr_idx=i, ...)`
- Update the TRs :`trbo_state.update_data_across_trs()`
  - What it does
    - `for i, tr in enumerate(self.trust_regions)`
      - Check if the global model is used.
      - Call [tr](#12-trustregion)`.update()`
- `trbo_state.TR_index_history.fill_(-2)`
- while Loop until reaches the `max_evals`
  - Thompson Sampling

<br><br>


## Classes and Functions
### 1. Trust Region Related Classes
#### 1.1 TurboHParams
- Location : `morbo.trust_region.TurboHParams`
- Members (Hyperparameters for TuRBO)
  - TR related
    - `eps: float = 1e-3`: The minimum percent improvement in objective that qualifying as a "success".
  - MOBO related
    - `hypervolume` : choose the type of TR between [ScalarizedTrustRegion](#121-scalarizedtrustregion) and [HypervolumeTrustRegion](#122-hypervolumetrustregion)
    - `winsor_pct` : manages winsorizing in [TrustRegion](#12-trustregion)`.update_model()` when fitting the GP model
    - `use_simple_rff` : enables using randomized fourier features
      - Disabled. Not supported for Botorch 0.15
#### 1.2 TrustRegion
- Location : `morbo.trust_region.TrustRegion`
- Members
  - `_objective` : The same `objective` as the [TRBOState](#21-trbostate) that owns it is given by [TRBOState](#21-trbostate).`initialize_standard()`
      - [ScalarizedTrustRegion](#121-scalarizedtrustregion) has different `objective()` function if `self.scalarization_weights` is not `None`.
      - [HypervolumeTrustRegion](#122-hypervolumetrustregion)'s `objective()` returns the above `self._objective`
- Methods
  - `__init__()`
      - Procedure
        - Set Members
          - `X`,`Y`,etc
        - `TrustRegion.update()`
          - `TrustRegion.update_model()`
            - `morbo.utils.get_fitted_model`
              - gpytorch optimization!
  - `update()`
    - Procedure
      - If data is added with `X_new` and `Y_new`
        - Add input `X_new` and `Y_new` to `self.X` and `self.Y` respectively.
        - Estimate $`f(X)`$ and update `self.Y_estimate`
          - In case of noisy turbo, call `self.update_model()` before the estimation.
        - `if update_streaks` (i.e. update TR length)
          - Calculate improvements with `_has_improved_objective()`
            - Differ by TR type
              - [ScalarizedTrustRegion](#121-scalarizedtrustregion)
              - [HypervolumeTrustRegion](#122-hypervolumetrustregion)
          - Count (success/failure) if an improvement (exists/does not exist).
          - If (success/failure) reaches the threshold, (double/halve) the length.
      - Estimate $`f(X)`$ and update `self.Y_estimate`
        - In case of noisy turbo, call `self.update_model()` before the estimation.
      - Update center and choose the best point: `self._update_center_and_best_points()`
        - Differs by the TR type
          - [ScalarizedTrustRegion](#121-scalarizedtrustregion)
          - [HypervolumeTrustRegion](#122-hypervolumetrustregion)

  - `update_model()`
    - Purpose: Update GP model of the current TR
    - Procedure
      - Normalize and standardize $`X`$ w.r.t. [BenchmarkFunction](#31-benchmarkfunction)'s bounds
      - Call `morbo.utils.get_fitted_model`
        - cf.) This method calls gpytorch optimization functions.
- Children Classes
  - [ScalarizedTrustRegion](#121-scalarizedtrustregion)
  - [HypervolumeTrustRegion](#122-hypervolumetrustregion)

#### 1.2.1 ScalarizedTrustRegion
- Location: `morbo.trust_region.ScalarizedTrustRegion`
- Members
  - `extra_buffers={"scalarization_weights": weights}`
    - This one is used to set `_scalarization_objective` below.
  - `_scalarization_objective`
    - Not set until `self.objective()` is called.
- Methods
  - `objective()` 
    - When first called, it sets the member `_scalarization_objective` by...
      - `self._gen_objective()` is called
      - If `self.scalarization_weights` is not `None`, call `get_objective_weights_transform()` to set it as the `self._scalarization_objective`
        - The weight is given by the `extra_buffers`
        - cf.) `get_objective_weights_transform` if from `botorch.utils.objective`.
    - Otherwise, return `self._scalarization_objective`
  - `_has_improved_objective`
    - Return `True` if
      - `self.objective(Y_new)` is greater than `self.objective(Y_prev)` + `self.eps` * `self.objective(Y_prev)`
  - `_update_center_and_best_points`
    - Set `obj` as the `Y_estimate`.
    - If the problem has constraint, call `get_constraint_slack_and_feasibility()` to add constraints.
      - Location: `morbo.utils.get_constraint_slack_and_feasibility`
    - Set center to be the one that maximize `obj`.
      - `center_idx = obj.argmax().item()`
    - Save by calling `self._set_center_and_best_points()`.

#### 1.2.2 HypervolumeTrustRegion
- Location: `morbo.trust_region.HypervolumeTrustRegion`
- Methods
  - `_has_improved_objective`
    - Use the `DominatedPartitioning` object to calculate the HVI.
      - cf.) `botorch.utils.multi_objective.box_decompositions.dominated.DominatedPartitioning`
  - `_update_center_and_best_points`

<br>

### 2. MOBO Management Related Classes
#### 2.1 TabuSet
- Location: `morbo.state.TabuSet`
  
#### 2.1 TRBOState
- Location: `morbo.state.TRBOState`  
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
- Methods
  - `update()`
    - Purpose
      - Update datapoints of the current MORBO 
      - Count # of evaluations
      - ([HVTR](#122-hypervolumetrustregion) only!) Calculate improvement and update pareto front.
    - Procedure
      - Add `X` and `Y` to `X_history` and `Y_history` respectively.
      - Increment the number of evaluations
        - `self.n_evals.add_(X.shape[0])`
      - If the TR is `HypervolumeTrustRegion`, update `pareto_X`, `pareto_Y`, and `hv`.
        - How?)
          - Add `X` and `Y` to `pareto_X` and `pareto_Y` respectively.
          - Pick pareto dominating points using `from botorch.utils.multi_objective.pareto import is_non_dominated`
          - Calculate HVI using `self.hv_contributions = self.hv - partitioning.compute_hypervolume()`
            - where `partitioning = BoxDecompositionList(*partitionings)`
  - `initialize_standard(tr_idx=i, ...)`
    - Purpose: Update the i-th TR and GP model
    - Procedure
      - [TrustRegion](#12-trustregion)`.update()`
      - [TrustRegion](#12-trustregion)`.update_model()`
  - `gen_scalarization_weights()`
    - Purpose : Generate scalarization weight for creating a [ScalarizedTR](#121-scalarizedtrustregion)

<br>

### 3. Benchmark Evaluation Function
#### 3.1 BenchmarkFunction
  - `morbo.benchmark_function.BenchmarkFunction`



[Back to Main](../../main.md)