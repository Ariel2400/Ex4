//#include <iostream>
#include <iostream>

#include "DFS_matrix_searcher.hpp"

#define BLOCK 0
#define VISITED 1
#define NOT_VISITED 0

DFSMatrixSearcher::DFSMatrixSearcher() {}

DFSMatrixSearcher::DFSMatrixSearcher(const DFSMatrixSearcher &other) {}

DFSMatrixSearcher &DFSMatrixSearcher::operator=(const DFSMatrixSearcher &other) {
    return *this;
}

bool DFSMatrixSearcher::is_there_path(const Problem &problem, int i, int j, std::unique_ptr<Matrix> &visited, std::string* solution, int* weight) {
    std::unique_ptr<Matrix> matrix = std::make_unique<Matrix>(*(problem.matrix));
    int height = matrix->get_height();
    int width = matrix->get_width();
    if (i >= 0 && i < height && j >= 0 && j < width && matrix->get_value(i, j) != BLOCK && visited->get_value(i, j) == NOT_VISITED) {
        visited->set_value(i, j, VISITED);
        if (i == problem.end_row && j == problem.end_column) {
            *weight += matrix->get_value(i, j);
            return true;
        }
        if (is_there_path(problem, i - 1, j, visited, solution)) {
            *solution = "Up," + *solution;
            *weight += matrix->get_value(i, j);
            return true;
        }
        if (is_there_path(problem, i + 1, j, visited, solution)) {
            *solution = "Down," + *solution;
            *weight += matrix->get_value(i, j);
            return true;
        }
        if (is_there_path(problem, i, j - 1, visited, solution)) {
            *solution = "Left," + *solution;
            *weight += matrix->get_value(i, j);
            return true;
        }
        if (is_there_path(problem, i, j + 1, visited, solution)) {
            *solution = "Right," + *solution;
            *weight += matrix->get_value(i, j);
            return true;
        }
    }
    return false;
}

SearchStatus DFSMatrixSearcher::search(const Problem &problem, std::string* solution, int* weight) {
    std::unique_ptr<Matrix> matrix = std::make_unique<Matrix>(*(problem.matrix));
    int height = matrix->get_height();
    int width = matrix->get_width();
    if (problem.start_row < 0 || problem.start_row >= height
        || problem.start_column < 0 || problem.start_column >= width
        || problem.end_row < 0 || problem.end_row >= height
        || problem.end_column < 0 || problem.end_column >= width) {
        return OUT_OF_BOUNDS_INDEX;
    }
    if (problem.start_row == problem.end_row && problem.start_column == problem.end_column) {
        *solution = "";
        *weight = 0;
        return PATH_FOUND;
    }
    std::unique_ptr<Matrix> visited = std::make_unique<Matrix>(height, width);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            visited->set_value(i, j, NOT_VISITED);
        }
    }
    *solution = "";
    *weight = 0;
    if (is_there_path(problem, problem.start_row, problem.start_column, visited, solution, weight)) {
        (*solution).pop_back();
        return PATH_FOUND;
    }
    return PATH_NOT_FOUND;
}

DFSMatrixSearcher::~DFSMatrixSearcher(){}

/*
int main() {
    std::unique_ptr<DFSMatrixSearcher> searcher = std::make_unique<DFSMatrixSearcher>();
    Problem problem = {std::make_unique<Matrix>(4, 4), 1, 0, 0, 1};
    for (int i = 0; i < problem.matrix->get_height(); ++i) {
        for (int j =0; j < problem.matrix->get_width(); ++j) {
            problem.matrix->set_value(i, j, 2);
        }
    }
    problem.matrix->set_value(0, 0, 0);
    problem.matrix->set_value(1, 1, 0);
    problem.matrix->set_value(2, 2, 0);
    problem.matrix->set_value(0, 3, 0);
    problem.matrix->set_value(6, 7, 0);
    //problem.matrix->set_value(3, 2, 0);
    std::string solution;
    SearchStatus status = searcher->search(problem, &solution);
    std::cout << solution << std::endl;
    std::cout << status << std::endl;
}
*/

int main() {
    std::unique_ptr<Matrix> matrix = std::make_unique<Matrix>(4, 4);
    matrix->set_value(-1, 2, 0);
}
