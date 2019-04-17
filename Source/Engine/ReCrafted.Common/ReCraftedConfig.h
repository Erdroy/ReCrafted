// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#define RC_YES      1
#define RC_NO       0
#define RC_ENABLE   RC_YES
#define RC_DISABLE  RC_NO

#ifndef FMT_HEADER_ONLY
# define FMT_HEADER_ONLY 
#endif


// ==== ASSERT SETTINGS ====

#ifndef ASSERT_FAIL_SHOW_MESSAGE_BOX
 /**
 * \brief Enables or disables message box, when assert fails
 */
# define ASSERT_FAIL_SHOW_MESSAGE_BOX    RC_DISABLE
#endif

#ifndef ASSERT_FAIL_DEBUG_BREAK
 /**
 * \brief Allows to break, when assert fails
 */
# define ASSERT_FAIL_DEBUG_BREAK         RC_DISABLE
#endif

#ifndef ASSERT_FAIL_EXCEPTION
 /**
 * \brief Enables or disables exception throw, when assert fails
 * When disabled, program normally exits with '-1' exit code.
 * When enabled, exception is being thrown which can be handled.
 */
# define ASSERT_FAIL_EXCEPTION           RC_ENABLE
#endif


// ==== ACTORS SETTINGS ====

#ifndef ACTOR_POOL_INITIAL_COUNT
 /**
  * \brief The initial amount of free actors in the pool.
  */
#define ACTOR_POOL_INITIAL_COUNT 128
#endif

#ifndef ACTOR_POOL_INCREMENT_COUNT
  /**
   * \brief The increment amount of actors. This much actors are spawned,
   *  when no free actors are present and we're trying to spawn new ones.
   */
#define ACTOR_POOL_INCREMENT_COUNT 16
#endif