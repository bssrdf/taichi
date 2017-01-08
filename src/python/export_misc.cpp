#include <taichi/python/export.h>
#include <taichi/python/exception.h>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/exception_translator.hpp>

using namespace boost::python;
namespace py = boost::python;

TC_NAMESPACE_BEGIN

Config config_from_py_dict(py::dict &c) {
    Config config;
    py::list keys = c.keys();
    for (int i = 0; i < len(keys); ++i) {
        py::object curArg = c[keys[i]];
        std::string key = py::extract<std::string>(keys[i]);
        std::string value = py::extract<std::string>(c[keys[i]]);
        config.set(key, value);
    }
    config.print_all();
    return config;
}

void test();

void translate_exception_for_python(const ExceptionForPython & e)
{
    PyErr_SetString(PyExc_RuntimeError, e.what());
}

void test_raise_error() {
    raise_assertion_failure_in_python("Just a test.");
}

void export_misc() {
    register_exception_translator<ExceptionForPython>(&translate_exception_for_python);
    def("test", test);
    def("test_raise_error", test_raise_error);
    def("config_from_dict", config_from_py_dict);
}

TC_NAMESPACE_END
