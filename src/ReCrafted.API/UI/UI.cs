using ReCrafted.API.Mathematics;

namespace ReCrafted.API.UI
{
    /// <summary>
    ///	UI internal class.
    /// </summary>
    internal static partial class UIInternal
    {
        /// <summary>
        /// Begins view rect.
        /// </summary>
        /// <param name="rectangle">Rectangle.</param>
        internal static void BeginViewRect(RectangleF rectangle)
        {
            UpdateViewRect(ref rectangle, true);
        }

        /// <summary>
        /// Ends view rect.
        /// </summary>
        internal static void EndViewRect()
        {
            var rect = new RectangleF(0f, 0f, 0f, 0f);
            UpdateViewRect(ref rect, false);
        }
    }
}
