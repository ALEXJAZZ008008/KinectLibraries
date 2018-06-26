#include <iostream>


class TestLibrary
{
public: 
    //! Default constructor
  explicit TestLibrary();
  
  //! Destructor, outputs a diagnostic message
  virtual ~TestLibrary();

  //! Function (to be overloaded) which does the actual tests.
  /*! This function is expected to do a series of calls to check(), check_if_equal() etc. */
  virtual void run_tests() = 0;
  
  //! Returns if all checks were fine upto now.
  bool is_everything_ok() const;

  int get_result() const;

 private:
  bool everything_ok;
    
};
    
TestLibrary::TestLibrary():
everything_ok(true)
{}

TestLibrary::~TestLibrary()
{

  if (everything_ok)
    std::cerr << "\nAll tests ok !\n\n" << std::endl;
  else
    std::cerr << "\nEnd of tests. Please correct errors !\n\n" << std::endl;
}

bool TestLibrary::is_everything_ok() const
{ return everything_ok ; }

int TestLibrary::get_result() const
{
    return everything_ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
