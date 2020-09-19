template <typename Problem, typename Solution>
class Solver {
    virtual Solution solve(Problem problem) = 0;
};