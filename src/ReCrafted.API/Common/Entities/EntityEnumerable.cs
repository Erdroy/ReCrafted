// ReCrafted (c) 2016-2018 Always Too Late

using System.Collections;
using System.Collections.Generic;

namespace ReCrafted.API.Common.Entities
{
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

        public unsafe IEnumerator<Entity> GetEnumerator()
        {
            return new EntityEnumerator(_ptrEntity, _numEntity);
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }
    }
}
