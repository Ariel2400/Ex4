template <class Problem, class Solution> class Solver {
    virtual Solution solve(Problem problem) = 0;
};