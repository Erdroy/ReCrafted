// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef RECRAFTEDCONFIG_H
#define RECRAFTEDCONFIG_H

#define RC_YES      1
#define RC_NO       0
#define RC_ENABLE   RC_YES
#define RC_DISABLE  RC_NO


// ==== COMPILE SETTINGS ====

#ifndef COMPILE_WITH_GRAPHICS
/**
 * \brief Enables or disables graphics code usage
 */
#define COMPILE_WITH_GRAPHICS           RC_ENABLE
#endif

#ifndef COMPILE_WITH_PHYSICS
/**
 * \brief Enables or disables physics code usage
 */
#define COMPILE_WITH_PHYSICS            RC_ENABLE
#endif

#ifndef COMPILE_WITH_WEBUI
/**
 * \brief Enables or disables WebUI code usage
 */
#define COMPILE_WITH_WEBUI              RC_ENABLE
#endif


 // ==== OTHER SETTINGS ====

#ifndef ASSERT_FAIL_SHOW_MESSAGE_BOX
 /**
 * \brief Enables or disables message box, when assert fails
 */
#define ASSERT_FAIL_SHOW_MESSAGE_BOX    RC_ENABLE
#endif

#ifndef ASSERT_FAIL_DEBUG_BREAK
 /**
 * \brief Allows to break, when assert fails
 */
#define ASSERT_FAIL_DEBUG_BREAK         RC_ENABLE
#endif

#ifndef ASSERT_FAIL_EXCEPTION
 /**
 * \brief Enables or disables exception throw, when assert fails
 * When disabled, program normally exits with '-1' exit code.
 * When enabled, eception is being thrown which can be handled.
 */
#define ASSERT_FAIL_EXCEPTION           RC_ENABLE
#endif


// ==== ACTOR POOL SETTINGS ====

#ifndef ACTOR_POOL_INITIAL_ACTOR_COUNT
 /**
  * \brief The initial amount of free actors in the pool.
  */
#define ACTOR_POOL_INITIAL_ACTOR_COUNT 1024
#endif

#ifndef ACTOR_POOL_INCREMENT_ACTOR_COUNT
  /**
   * \brief The increment amount of actors. This much actors are spawned,
   *  when no free actors are present and we're trying to spawn new one.
   */
#define ACTOR_POOL_INCREMENT_ACTOR_COUNT 16
#endif

// ==== ECS SETTINGS ====

#ifndef ECS_MAX_SYSTEMS
/**
 * \brief The maximum amount of ECS systems.
 */
#define ECS_MAX_SYSTEMS 64
#endif

#ifndef ECS_MAX_NATIVE_SYSTEMS
 /**
  * \brief The maximum amount of native ECS systems. The rest will be available for .NET.
  */
#define ECS_MAX_NATIVE_SYSTEMS 32
#endif

#ifndef ECS_MAX_COMPONENTS
 /**
  * \brief The maximum amount of ECS components.
  */
#define ECS_MAX_COMPONENTS 128
#endif

#ifndef ECS_MAX_NATIVE_COMPONENTS
  /**
   * \brief The maximum amount of native ECS components. The rest will be available for .NET.
   */
#define ECS_MAX_NATIVE_COMPONENTS 32
#endif

#ifndef ECS_MAX_COMPONENT_SIZE
   /**
    * \brief The maximum size of an ECS component.
    */
#define ECS_MAX_COMPONENT_SIZE 512
#endif

#endif // RECRAFTEDCONFIG_H
