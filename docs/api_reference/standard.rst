
kaguya::standard namespace
==================================

This namespace store standard libraries.

using boost library for not enough in C++03.

In this way it has been defined in kaguya/config.hpp

.. code-block:: c++

  namespace kaguya
  {
    namespace standard
    {
    #if KAGUYA_USE_CPP11
      using namespace std;
    #else
      using namespace boost;
    #endif
    }
  }


Used switched types
-----------------------------

* tuple

  using multiple return value.

* shared_ptr

  using shared_ptr assign. see :ref:`smartpointers<class-bindings-smartpointers>`.
