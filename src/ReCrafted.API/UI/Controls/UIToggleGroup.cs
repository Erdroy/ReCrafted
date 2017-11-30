// ReCrafted © 2016-2017 Always Too Late

using System;
using System.Collections.Generic;
using System.Linq;

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
        /// <summary>
        /// Creates new UIToggleGroup.
        /// </summary>
        public UIToggleGroup()
        {

        }

        /// <summary>
        /// Creates new UIToggleGroup.
        /// </summary>
        /// <param name="min">Minimum value of enabled toggles.</param>
        public UIToggleGroup(int min)
        {
            Minimum = min;
        }

        /// <summary>
        /// Creates new UIToggleGroup.
        /// </summary>
        /// <param name="min">Minimum value of enabled toggles.</param>
        /// <param name="max">Maximum value of enabled toggles.</param>
        public UIToggleGroup(int min, int max)
        {
            Minimum = min;
            Maximum = max;
        }

        // all added toggles in to this group
        private readonly List<UIToggle> _uiToggles = new List<UIToggle>();
        // sorted list of all enabled toggles
        private readonly List<UIToggle> _uiTogglesQueue = new List<UIToggle>();

        // minimum of enabled toggles.
        private int _minimum;
        // maximum of enable toggles.
        private int _maximum;

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
                OnMemberValueChanged(toggle, value);
                OnValueChanged?.Invoke(toggle, value);
            };
            _uiToggles.Add(toggle);
            if (GetSelectedCount() < Minimum)
            {
                toggle.IsOn = true;
                _uiTogglesQueue.Add(toggle);
            }
            _updateToggles();
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
            if (_uiTogglesQueue.Contains(toggle))
                _uiTogglesQueue.Remove(toggle);
            _updateToggles();
        }

        /// <summary>
        /// Gets count of selected toggles.
        /// </summary>
        public int GetSelectedCount()
        {
            return _uiTogglesQueue.Count(toggle => toggle.IsOn);
        }

        // set false in all added toggles except given
        private void OnMemberValueChanged(UIToggle toggle, bool value)
        {
            // update queue
            if (_uiTogglesQueue.Contains(toggle))
                _uiTogglesQueue.Remove(toggle);
            if (value)
                _uiTogglesQueue.Add(toggle);
            else
            {
                if (_uiTogglesQueue.Count == 0)
                {
                    var i = _uiToggles.IndexOf(toggle);
                    if (i >= _uiToggles.Count-1)
                        _uiTogglesQueue.Add(_uiToggles[0]);
                    else
                    {
                        _uiTogglesQueue.Add(_uiToggles[i + 1]);
                    }
                }
            }

            // default
            if (_minimum == 0 && _maximum == 0)
            {
                foreach (var t in _uiToggles)
                {
                    if (t == toggle)
                        continue;
                    t.IsOn = false;
                }
            }
            else
            {
                _updateToggles();
            }
        }

        // updates current toggles.
        private void _updateToggles()
        {
            if (_minimum == 0 && _maximum == 0)
                return;


            int total = 0;
            for (int index = _uiTogglesQueue.Count - 1; index >= 0; index--)
            {
                if (_uiTogglesQueue[index].IsOn)
                    total++;
            }

            if (total < Minimum)
            {
                for (int index = _uiTogglesQueue.Count - 1; index >= 0; index--)
                {
                    if (total >= Minimum)
                        break;
                    if (_uiTogglesQueue[index].IsOn) continue;
                    _uiTogglesQueue[index].IsOn = true;
                    total++;
                }
            }

            total = 0;
            for (int index = _uiTogglesQueue.Count - 1; index >= 0; index--)
            {
                if (total >= _maximum)
                {
                    if (_uiTogglesQueue[index].IsOn)
                    {
                        _uiTogglesQueue[index].IsOn = false;
                        total--;
                    }
                }

                if (_uiTogglesQueue[index].IsOn)
                    total++;
            }
        }

        /// <summary>
        /// Defines minimum of controls that need to be enabled.
        /// </summary>
        public int Minimum
        {
            set
            {
                _minimum = value;
                _updateToggles();
            }
            get { return _minimum; }
        }

        /// <summary>
        /// Defines maximum of controls that can be enabled.
        /// </summary>
        public int Maximum
        {
            set
            {
                _maximum = value;
                if (_maximum < _minimum)
                    _maximum = _minimum;
                _updateToggles();
            }
            get { return _maximum; }
        }
    }
}
