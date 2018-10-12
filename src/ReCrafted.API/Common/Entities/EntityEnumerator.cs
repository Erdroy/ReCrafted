// ReCrafted (c) 2016-2018 Always Too Late

using System.Collections;

namespace ReCrafted.API.Common.Entities
{
    /// <inheritdoc />
    /// <summary>
    /// EntityEnumerator implementation. Implements iteration over native Entity* list.
    /// </summary>
    public class EntityEnumerator : IEnumerator
    {
        private readonly unsafe Entity* _ptrEntity;
        private readonly int _numEntity;

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
        object IEnumerator.Current => Current;

        /// <summary>
        /// Reference to the current Entity instance.
        /// </summary>
        public ref Entity Current
        {
            get
            {
                unsafe
                {
                    return ref _ptrEntity[_position];
                }
            }
        }
    }
}
