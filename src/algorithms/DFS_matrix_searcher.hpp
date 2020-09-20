#include <string>

#include "matrix_searcher.hpp"
#include "../matrices/matrix_class.hpp"

class DFSMatrixSearcher: public MatrixSearcher {
    public:
        DFSMatrixSearcher();
        DFSMatrixSearcher(const DFSMatrixSearcher &other);
        DFSMatrixSearcher &operator=(const DFSMatrixSearcher &other);
        virtual SearchStatus search(const Problem &problem , std::string* solution);
        ~DFSMatrixSearcher();
    private:
        bool is_there_path(const Problem &problem, int i, int j, std::unique_ptr<Matrix> &visited, std::string* solution, int* weight);
};