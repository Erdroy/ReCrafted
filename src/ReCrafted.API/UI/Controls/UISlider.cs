// ReCrafted © 2016-2017 Always Too Late

namespace ReCrafted.API.UI.Controls
{
    /// <summary>
    /// UI Slider class.
    /// </summary>
    public class UISlider : UIControl
    {
        // draw
        public override void Draw()
        {

        }

        // reset
        public override void Reset()
        {

        }

        /// <summary>
        /// Current value of slider.
        /// </summary>
        public float Value { get; set; }

        /// <summary>
        /// Minimum value of slider.
        /// </summary>
        public float Minimum { get; set; }

        /// <summary>
        /// Maximum value of slider.
        /// </summary>
        public float Maximum { get; set; }

        /// <summary>
        /// Only whole numbers can be used by this slider.
        /// </summary>
        public bool WholeNumbers { get; set; } = false;
    }
}
