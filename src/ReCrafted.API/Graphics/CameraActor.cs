using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ReCrafted.API.Graphics
{
    public partial class CameraActor
    {
        /// <summary>
        /// Creates new actor of type 'CameraActor'.
        /// </summary>
        /// <returns>The newly created actor.</returns>
        public static CameraActor Create()
        {
            return New<CameraActor>();
        }
    }
}
