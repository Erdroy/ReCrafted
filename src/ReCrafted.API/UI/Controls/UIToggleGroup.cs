// ReCrafted © 2016-2017 Always Too Late

using System;
using System.Collections.Generic;

namespace ReCrafted.API.UI.Controls
{
    /// <summary>
    /// Delegate for toggle group value change event.
    /// </summary>
    /// <param name="toggle">Current toggle.</param>
    /// <param name="value">Is current toggle active.</param>
    public delegate void ToggleGroupValueChanged(UIToggle toggle, bool value);

    /// <summary>
    /// UIToggle group.
    /// </summary>
    public class UIToggleGroup
    {
        // all added toggles in to this group
        private readonly List<UIToggle> _uiToggles = new List<UIToggle>();

        /// <summary>
        /// When active toggle in group has been changed.
        /// </summary>
        public ToggleGroupValueChanged OnValueChanged;

        /// <summary>
        /// Adds given toggle in to this group.
        /// </summary>
        /// <param name="toggle">Our toggle to add.</param>
        /// <exception cref="ArgumentNullException">Exception is thrown when given instance of control is null.</exception>
        /// <returns>Our given toggle.</returns>
        public UIToggle Add(UIToggle toggle)
        {
            if (toggle == null) throw new NullReferenceException(nameof(toggle));
            if (_uiToggles.Contains(toggle)) return null;
            toggle.OnValueChanged = null;
            toggle.OnValueChanged += value =>
            {
                if (value)
                OnMemberValueChanged(toggle);
                OnValueChanged?.Invoke(toggle, value);
            }; 
            _uiToggles.Add(toggle);
            return toggle;
        }

        /// <summary>
        /// Removes given toggle from group.
        /// </summary>
        /// <param name="toggle">Our toggle to remove.</param>
        /// <exception cref="ArgumentNullException">Exception is thrown when given instance of control is null.</exception>
        public void Remove(UIToggle toggle)
        {
            if (toggle == null) throw new NullReferenceException(nameof(toggle));
            if (!_uiToggles.Contains(toggle)) return;
            toggle.OnValueChanged = null;
            _uiToggles.Remove(toggle);
        }

        // set false in all added toggles except given
        private void OnMemberValueChanged(UIToggle toggle)
        {
            foreach (var t in _uiToggles)
            {
                if (t == toggle) continue;
                t.IsOn = false;
            }
        }
    }
}
