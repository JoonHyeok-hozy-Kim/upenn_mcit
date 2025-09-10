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
        - Update GP model, adjust TR, and update center
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
  - `_set_center_and_best_points()`
    - Purpose) Set
      - `self.X_center`, `self.Y_center`, 
      - `self.best_X = self.X_center`
      - `self.best_Y = self.Y_center`, 
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
      ```python
      center_idx = obj.argmax().item()
      ```
    - Save by calling `self._set_center_and_best_points()`.

#### 1.2.2 HypervolumeTrustRegion
- Location: `morbo.trust_region.HypervolumeTrustRegion`
- Methods
  - `_has_improved_objective`
    - Use the `DominatedPartitioning` object to calculate the HVI.
      - cf.) `botorch.utils.multi_objective.box_decompositions.dominated.DominatedPartitioning`
  - `_update_center_and_best_points`
    - Procedure)
      - Update current best $`X,Y`$, reference point, and HV.
      - If `X_center` is given, 
        - find that row in `self.X` and save that index by calling `_set_center_and_best_points`
      - Otherwise, get new center as...
        - Mask indices of target points in $`X`$ as `eligible_mask`
          - where 
            - `eligible_mask = indices_mask & not_taken_mask`
            - `indices_mask` : Indices within this TR.
              - cf.) [get_indices_in_hypercube](#44-get_indices_in_hypercube) is used.
            - `not_taken_mask` : Indices of points that are not (current best and included in `invalid_centers`)
        - Let `self.X_center` be the point from `eligible_mask` that has the best value in caller `TRBOState.hv_contribution`.
          - cf.) `base_idx = hv_contributions[eligible_indices].argmax()`

<br>

### 2. MOBO Management Related Classes
#### 2.1 TabuSet
- Location: `morbo.state.TabuSet`
  
#### 2.1 TRBOState
- Location: `morbo.state.TRBOState`  
- Members
  - TR related
    - `trust_regions` : a list that contains [TR object](#12-trustregion)s.
      - Each [TR object](#12-trustregion) contains the `model` member
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
    - `self.hv`
      - Scalar datatype
      - Updated by `TRBOState.update()`
    - `self.hv_contribution`
      - `BoxDecompositionList` datatype that contains HVI for each point on the Pareto Front
      - Updated by `TRBOState.update()`
- Methods
  - `update()`
    - Purpose
      - Update datapoints of the current MORBO : `self.X_history`, `self.Y_history` 
      - Count # of evaluations: `self.n_evals`
      - ([HVTR](#122-hypervolumetrustregion) only!) Calculate improvement and update pareto front: `self.hv`, `self.hv_contribution`
    - Procedure
      - Add `X` and `Y` to `X_history` and `Y_history` respectively.
      - Increment the number of evaluations
        - `self.n_evals.add_(X.shape[0])`
      - If the TR is `HypervolumeTrustRegion`, update `pareto_X`, `pareto_Y`, and `hv`.
        - How?)
          - Add `X` and `Y` to `pareto_X` and `pareto_Y` respectively.
          - Pick pareto dominating points using `from botorch.utils.multi_objective.pareto import is_non_dominated`
          - Calculate HVI using `self.hv_contributions = self.hv - partitioning.compute_hypervolume()`
            - How?)
              - `batch_paretos=` $`N\times \underbrace{(N-1)\times m}_{\text{PF points that exclude itself}}`$ Tensor 
                - where $`N=\vert\mathcal{PF}\vert`$
                - i.e.) PF points that exclude itself for each PF point
              - Calculate HV for each PF point, subtract it from `self.hv`, and store it in `self.hv_contributions`
                - `self.hv` is scalar.
                - `self.hv_contributions` is $`N\times`$
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

<br>

### 4. Utilities
- Location : `morbo.utils.py`
#### 4.1 sample_tr_discrete_points

#### 4.2 sample_tr_discrete_points_subset_d

#### 4.3 get_tr_center

#### 4.4 get_indices_in_hypercube
- Purpose)
  - Given `X_center` and `X`, return the indices of `X` that are included in the TR centered at `X_center`.
- Return val
  `Tensor[indices of X]` in $(k,)$-shape
  - e.g.) [0,2,5,10]

#### 4.5 get_fitted_model

#### 4.6 coalesce

#### 4.7 decay_function

#### 4.8 get_constraint_slack_and_feasibility

<br>

### 5. Generators
#### 5.1 TS_select_batch_MORBO
- Purpose
  - Get candidates for the next batch in greedy fashion
  - Refer to [Collaborative Batch Selection via Global Utility Maximization](./morbo_paper_summary.md#concept-collaborative-batch-selection-via-global-utility-maximization) in MORBO
- Return `CandidateSelectionOutput` s.t.
  ```python
  class CandidateSelectionOutput(NamedTuple):
      X_cand: Tensor
      tr_indices: Tensor
  ```
- Procedure
  - `for i in batch_size`
    - Let `tr_indices` be
      - each indices of TRs for [HVTR](#122-hypervolumetrustregion)
      - randomly chosen index of TR for [ScalarizationTR](#121-scalarizedtrustregion)
    - `for tr_idx in tr_indices`
      - Let `best_X` be indices of the normalized values of `tr.best_X`.


[Back to Main](../../main.md)