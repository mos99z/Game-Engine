using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace CSharpTestClass
{
   // [ComVisible(true)]
    public interface ITest
    {
        int Get();
    }
   // [ComVisible(true)]
   // [ClassInterface(ClassInterfaceType.None)]
    public class Test :ITest
    {
        public int Get()
        {
            return 42;
        }
    }
}
