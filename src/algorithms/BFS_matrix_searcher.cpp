#include <queue>
#include <iostream>

#include "BFS_matrix_searcher.hpp"

struct Step {
    public:
        int row;
        int column;
        std::string path;

        Step(int row, int column, std::string path) {
            this->row = row;
            this->column = column;
            this->path = path;
        }
};

BFSMatrixSearcher::BFSMatrixSearcher() {}

BFSMatrixSearcher::BFSMatrixSearcher(const BFSMatrixSearcher &other) {}

BFSMatrixSearcher &BFSMatrixSearcher::operator=(const BFSMatrixSearcher &other) {
    return *this;
}

SearchStatus BFSMatrixSearcher::search(const Problem &problem, std::string* solution) {
    std::unique_ptr<Matrix> matrix = std::make_unique<Matrix>(*(problem.matrix));
    int height = matrix->get_height();
    int width = matrix->get_width();
    if (problem.start_row < 0 || problem.start_row >= height
        || problem.start_column < 0 || problem.start_column >= width
        || problem.end_row < 0 || problem.end_row >= height
        || problem.end_column < 0 || problem.end_column >= width) {
        return OUT_OF_BOUNDS_INDEX;
    }
    std::queue<Step> step_queue;
    std::unique_ptr<Matrix> visited = std::make_unique<Matrix>(height, width);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (matrix->get_value(i, j) == 0) {
                visited->set_value(i, j, 1);
            } else {
                visited->set_value(i, j, 0);
            }
            if (i == problem.start_row && j == problem.start_column) {
                step_queue.push(Step(i, j, ""));
                visited->set_value(i, j, 1);
            }
        }
    }
    while (!step_queue.empty()) {
        Step step = step_queue.front();
        step_queue.pop();
        if (step.row == problem.end_row && step.column == problem.end_column) {
            if (step.path.length() != 0) {
                step.path.pop_back();
            }
            *solution = step.path;
            return PATH_FOUND;
        }
        if (step.row - 1 >= 0 && visited->get_value(step.row - 1, step.column) == 0) { 
            step_queue.push(Step(step.row - 1, step.column, step.path + "Up,")); 
            visited->set_value(step.row - 1, step.column, 1); 
        }
        if (step.row + 1 < height && visited->get_value(step.row + 1, step.column) == 0) { 
            step_queue.push(Step(step.row + 1, step.column, step.path + "Down,")); 
            visited->set_value(step.row + 1, step.column, 1); 
        }
        if (step.column - 1 >= 0 && visited->get_value(step.row, step.column - 1) == 0) { 
            step_queue.push(Step(step.row, step.column - 1, step.path + "Left,")); 
            visited->set_value(step.row, step.column - 1, 1); 
        }
        if (step.column + 1 < width && visited->get_value(step.row, step.column + 1) == 0) { 
            step_queue.push(Step(step.row, step.column + 1, step.path + "Right,")); 
            visited->set_value(step.row, step.column + 1, 1); 
        }
    }
    return PATH_NOT_FOUNT;
}

BFSMatrixSearcher::~BFSMatrixSearcher(){}

/*
int main() {
    std::unique_ptr<BFSMatrixSearcher> searcher = std::make_unique<BFSMatrixSearcher>();
    Problem problem = {std::make_unique<Matrix>(4, 4), 0, 0, 3, 0};
    for (int i = 0; i < problem.matrix->get_height(); ++i) {
        for (int j =0; j < problem.matrix->get_width(); ++j) {
            problem.matrix->set_value(i, j, 2);
        }
    }
    problem.matrix->set_value(1, 0, 0);
    problem.matrix->set_value(1, 1, 0);
    problem.matrix->set_value(1, 2, 0);
    problem.matrix->set_value(1, 3, 0);
    problem.matrix->set_value(3, 3, 0);
    problem.matrix->set_value(3, 2, 0);
    std::string solution;
    SearchStatus status = searcher->search(problem, &solution);
    std::cout << solution << std::endl;
    std::cout << status << std::endl;
}
*/