using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ReCrafted.API.Common
{
    public sealed partial class Logger
    {
        public static void Log(string str)
        {
            InternalWriteLog(LogLevel.Message, str);
        }
    }
}
