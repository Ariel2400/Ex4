#include <string>
#include <queue>
#include <vector>

#include "matrix_searcher.hpp"
#include "../matrices/matrix_class.hpp"

struct Step {
    public:
        int row;
        int column;
        std::string path;
        int weight;

        Step(int row, int column, std::string path, int weight) {
            this->row = row;
            this->column = column;
            this->path = path;
            this->weight = weight;
        }
};

struct CompareWeight {
    bool operator()(Step const& step1, Step const& step2) {
        return step1.weight > step2.weight;
    }
};

class BestFSMatrixSearcher: public MatrixSearcher {
    public:
        BestFSMatrixSearcher();
        BestFSMatrixSearcher(const BestFSMatrixSearcher &other);
        BestFSMatrixSearcher &operator=(const BestFSMatrixSearcher &other);
        virtual SearchStatus search(const Problem &problem , std::string* solution, int* weight);
        ~BestFSMatrixSearcher();
    private:
        void update_in_queue(std::priority_queue<Step, std::vector<Step>, CompareWeight>* queue, Step newStep);
};