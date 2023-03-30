//
// Copyright (c) 2012-2021 Snowflake Computing Inc. All rights reserved.
//

#ifndef PC_ARROWITERATOR_HPP
#define PC_ARROWITERATOR_HPP

#include "Python/Common.hpp"
#include "logging.hpp"
#include "nanoarrow.hpp"
#include <cstdint>
#include <memory>
#include <string>
#include <vector>


static const char* NANOARROW_TYPE_ENUM_STRING[] = {
    "NANOARROW_TYPE_UNINITIALIZED",
    "NANOARROW_TYPE_NA",
    "NANOARROW_TYPE_BOOL",
    "NANOARROW_TYPE_UINT8",
    "NANOARROW_TYPE_INT8",
    "NANOARROW_TYPE_UINT16",
    "NANOARROW_TYPE_INT16",
    "NANOARROW_TYPE_UINT32",
    "NANOARROW_TYPE_INT32",
    "NANOARROW_TYPE_UINT64",
    "NANOARROW_TYPE_INT64",
    "NANOARROW_TYPE_HALF_FLOAT",
    "NANOARROW_TYPE_FLOAT",
    "NANOARROW_TYPE_DOUBLE",
    "NANOARROW_TYPE_STRING",
    "NANOARROW_TYPE_BINARY",
    "NANOARROW_TYPE_FIXED_SIZE_BINARY",
    "NANOARROW_TYPE_DATE32",
    "NANOARROW_TYPE_DATE64",
    "NANOARROW_TYPE_TIMESTAMP",
    "NANOARROW_TYPE_TIME32",
    "NANOARROW_TYPE_TIME64",
    "NANOARROW_TYPE_INTERVAL_MONTHS",
    "NANOARROW_TYPE_INTERVAL_DAY_TIME",
    "NANOARROW_TYPE_DECIMAL128",
    "NANOARROW_TYPE_DECIMAL256",
    "NANOARROW_TYPE_LIST",
    "NANOARROW_TYPE_STRUCT",
    "NANOARROW_TYPE_SPARSE_UNION",
    "NANOARROW_TYPE_DENSE_UNION",
    "NANOARROW_TYPE_DICTIONARY",
    "NANOARROW_TYPE_MAP",
    "NANOARROW_TYPE_EXTENSION",
    "NANOARROW_TYPE_FIXED_SIZE_LIST",
    "NANOARROW_TYPE_DURATION",
    "NANOARROW_TYPE_LARGE_STRING",
    "NANOARROW_TYPE_LARGE_BINARY",
    "NANOARROW_TYPE_LARGE_LIST",
    "NANOARROW_TYPE_INTERVAL_MONTH_DAY_NANO"
};


#define SF_CHECK_ARROW_RC(arrow_status, format_string, ...) \
  if (!arrow_status.ok()) \
  { \
    std::string errorInfo = Logger::formatString(format_string, ##__VA_ARGS__); \
    logger->error(__FILE__, __func__, __LINE__, errorInfo.c_str()); \
    PyErr_SetString(PyExc_Exception, errorInfo.c_str()); \
    return; \
  }

#define SF_CHECK_ARROW_RC_AND_RETURN(arrow_status, ret_val, format_string, ...) \
  if (!arrow_status.ok()) \
  { \
    std::string errorInfo = Logger::formatString(format_string, ##__VA_ARGS__); \
    logger->error(__FILE__, __func__, __LINE__, errorInfo.c_str()); \
    PyErr_SetString(PyExc_Exception, errorInfo.c_str()); \
    return ret_val; \
  }

namespace sf
{

/**
 * A simple struct to contain return data back cython.
 * PyObject would be nullptr if failed and cause string will be populated
 */
class ReturnVal
{
public:
  ReturnVal(PyObject * obj, PyObject *except) :
    successObj(obj), exception(except)
  {
  }

  PyObject * successObj;

  PyObject * exception;
};

/**
 * Arrow base iterator implementation in C++.
 */

class CArrowIterator
{
public:
  CArrowIterator(std::vector<std::shared_ptr<arrow::RecordBatch>> * batches);

  virtual ~CArrowIterator() = default;

  /**
   * @return a python object which might be current row or an Arrow Table
   */
  virtual std::shared_ptr<ReturnVal> next() = 0;
  virtual std::vector<uintptr_t> getArrowArrayPtrs() { return {}; };
  virtual std::vector<uintptr_t> getArrowSchemaPtrs() { return {}; };

protected:
   /** list of all record batch in current chunk */
  std::vector<std::shared_ptr<arrow::RecordBatch>> *m_cRecordBatches;

  static Logger* logger;
};
}

#endif  // PC_ARROWITERATOR_HPP
