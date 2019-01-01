// ReCrafted (c) 2016-2019 Always Too Late

using System.Collections;
using System.Collections.Generic;

namespace ReCrafted.API.Common.Entities
{
    /// <inheritdoc />
    /// <summary>
    /// ECS Entity Enumerable.
    /// </summary>
    public class EntityEnumerable : IEnumerable<Entity>
    {
        private readonly unsafe Entity* _ptrEntity;
        private readonly int _numEntity;

        /// <summary>
        /// Default constructor.
        /// </summary>
        internal unsafe EntityEnumerable(Entity* entitiesPtr, int entitiesNum)
        {
            _ptrEntity = entitiesPtr;
            _numEntity = entitiesNum;
        }

        /// <inheritdoc />
        public unsafe IEnumerator<Entity> GetEnumerator()
        {
            return new EntityEnumerator(_ptrEntity, _numEntity);
        }

        /// <inheritdoc />
        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }
    }
}
