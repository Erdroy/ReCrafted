// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System;
using System.Collections.Generic;
using ReCrafted.API.Common;

namespace ReCrafted.API.Core
{
    /// <inheritdoc />
    /// <summary>
    /// GameSystem class. Implements game system functionality.
    /// </summary>
    /// <remarks>
    /// GameSystem is different from ComponentSystem, as it has no support for Entities.
    /// </remarks>
    public class GameSystem : IDisposable
    {
        internal static List<GameSystem> GameSystems = new List<GameSystem>();

        protected virtual void OnCreate() { }
        protected virtual void OnDestroy() { }
        protected virtual void OnUpdate() { }

        /// <inheritdoc />
        public void Dispose()
        {
            if (IsDisposed)
                return;

            IsDisposed = true;
            OnDestroy();
        }

        /// <summary>
        /// Updates all systems.
        /// </summary>
        internal static void UpdateAll()
        {
            foreach (var system in GameSystems)
            {
                try
                {
                    system?.OnUpdate();
                }
                catch (Exception ex)
                {
                    Logger.LogError("Failed to update GameSystem!");
                    Exceptions.WriteException(ex);
                }
            }
        }
        
        /// <summary>
        /// Adds game system instance of given type.
        /// </summary>
        /// <returns>The game system instance.</returns>
        public static TGameSystem AddGameSystem<TGameSystem>() where TGameSystem : GameSystem, new()
        {
            // Register system
            return AddGameSystem<TGameSystem>(new TGameSystem());
        }

        /// <summary>
        /// Adds game system instance.
        /// </summary>
        /// <param name="system">The game system instance.</param>
        /// <returns>The game system instance.</returns>
        public static TGameSystem AddGameSystem<TGameSystem>(GameSystem system) where TGameSystem : GameSystem, new()
        {
            system.OnCreate();

            GameSystems.Add(system);
            return (TGameSystem)system;
        }
        
        /// <summary>
        /// Removes given game system.
        /// </summary>
        /// <param name="system">The game system instance.</param>
        public static void RemoveGameSystem(GameSystem system)
        {
            system.Dispose();
            GameSystems.Remove(system);
        }

        /// <summary>
        /// True when this game system has been disposed.
        /// </summary>
        public bool IsDisposed { get; private set; }
    }
}
