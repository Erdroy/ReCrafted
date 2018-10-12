// ReCrafted (c) 2016-2018 Always Too Late

using System.Collections;
using System.Collections.Generic;

namespace ReCrafted.API.Common.Entities
{
    /// <inheritdoc />
    /// <summary>
    /// EntityEnumerator implementation. Implements iteration over native Entity* list.
    /// </summary>
    public class EntityEnumerator : IEnumerator<Entity>
    {
        private unsafe Entity* _ptrEntity;
        private int _numEntity;

        private int _position;
        
        /// <summary>
        /// Default constructor.
        /// </summary>
        internal unsafe EntityEnumerator(Entity* entitiesPtr, int entitiesNum)
        {
            _ptrEntity = entitiesPtr;
            _numEntity = entitiesNum;
            _position = -1;
        }

        /// <inheritdoc />
        public bool MoveNext()
        {
            _position++;
            return _position < _numEntity;
        }

        /// <inheritdoc />
        public void Reset()
        {
            _position = -1;
        }

        /// <inheritdoc />
        public void Dispose()
        {
            unsafe
            {
                _ptrEntity = null;
                _numEntity = 0;
            }

            Reset();
        }

        /// <inheritdoc />
        object IEnumerator.Current => Current;

        /// <inheritdoc />
        /// <summary>
        /// Reference to the current Entity instance.
        /// </summary>
        public Entity Current
        {
            get
            {
                unsafe
                {
                    return _ptrEntity[_position];
                }
            }
        }
    }
}
