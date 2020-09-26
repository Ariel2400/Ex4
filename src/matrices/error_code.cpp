#include "error_code.hpp"

/**
 * @brief Checks if a given error code indicates a success or not.
 *
 * @param[in] code the error code.
 * @return whether the error code indicates a success or not.
 */
bool error_isSuccess(ErrorCode code) { return code == ERROR_SUCCESS; }

/**
 * @brief gets a textual error message for a given error code.
 *
 * @param[in] code the error code.
 * @return const char* the textual representation of the error code.
 */
const std::string error_getErrorMessage(ErrorCode code) {
  switch (code) {
  case ERROR_SUCCESS:
    return "The operation was performed successfully";
  case ERROR_MISSING_MATRIX:
    return "Error: input matrix pointer equals NULL";
  case ERROR_MISSING_MATRIX_HEIGHT:
    return "Error: input matrix height missing";
  case ERROR_MISSING_MATRIX_WIDTH:
    return "Error: input matrix width missing";
  case ERROR_MISSING_MATRIX_VALUES:
    return "Error: values missing from input matrix";
  case ERROR_NULL_OUTPUT_POINTER:
    return "Error: tried to store information into a NULL address";
  case ERROR_INVALID_MATRIX_SIZES:
    return "Error: invalid matrix size(s) for the requested operation";
  case ERROR_FAILED_MEMORY_ALLOCATION:
    return "Error: failed to allocate memory";
  default:
    return "Unidentified Error";
  }
}