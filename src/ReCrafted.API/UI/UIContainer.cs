// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

using System;
using System.Collections.Generic;

namespace ReCrafted.API.UI
{
    /// <summary>
    /// UIContainer class, implements support for containing child UIControls.
    /// </summary>
    public class UIContainer : UIControl
    {
        private readonly List<UIControl> _controls = new List<UIControl>();

        /// <summary>
        /// Draws all controls.
        /// </summary>
        public override void Draw()
        {
            throw new NotImplementedException();
        }
        
        /// <summary>
        /// Adds new control to the container and then returns it.
        /// </summary>
        /// <typeparam name="T">Control class which must inherit from UIControl.</typeparam>
        /// <param name="instance">The control instance.</param>
        /// <returns>The control instance, allows to modify preferences easily.</returns>
        public T Add<T>(T instance) where T : UIControl
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Removes given control from the container.
        /// </summary>
        /// <typeparam name="T">Control class which must inherit from UIControl.</typeparam>
        /// <param name="instance">The control instance.</param>
        public void Remove<T>(T instance) where T : UIControl
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Contains all the controls
        /// </summary>
        public UIControl[] Controls => _controls.ToArray();
    }
}
