# ReCrafted C++/C# code style
This style is based on conventions from 'ReSharper' software.

# Style and naming used while developing source code in C++

# 1. Comments
We use only one-line comments (except if they are used to disable some part of code), in which there are spaces after the beginning and text starts with lowercase.
Commas and dots are allowed to use.
Every class/struct or `public` function should get it's comment.

```CPP
// this is a comment,
// which is pretty cool.
```
Documentation/Member comments should look like this:
```CPP
/**
* \brief This is a member comment.
* \more tags goes here
*/
```
Each method parameter should get it's comment with `\param paramName paramComment`

# 2. Classes / Structs / Methods
For classes or structs, we are using `public:`, `private:` etc. to sepearate different member types, example:
```CPP
/**
* \brief SomeClass class description.
*/
class SomeClass
{
private:
  int m_someVariable = 0;

private:
  void privateFunction();

public:
  /**
  * \brief Some function.
  */
  void publicFunction(); 
}
```

# 3. Variables
Variables should always be private and start with **m_**`lowerCaseLetter` (if not PROPERTY and it is OWNED by the object) or start lowerCase if this variable is set by other object `void* somePointer = nullptr;`.
When variable needs to be exposed, use `PROPERTY` or `STATIC_PROPERTY`.

# 4. Enums
Similar to classes/structs.
We are using struct to encapsulate the enum for easier usage, where struct name is the enum name, and the enum definition name is always `_enum`.
```CPP
/**
* \brief SomeEnum enum description.
*/
struct SomeEnum
{
public:
  enum _enum
  {
   First,
   Second,
   Third
  }
}
```

# 8. Layout/Members order in header files

```CPP
// ReCrafted © 2016-2017 Always Too Late

// includes
#include "someHeader.h"

/**
* \brief Description.
*/
[MODIFIER] Name
{
private:
  /**
  * \brief Description.
  */
  struct InnerStruct{};

public:
  /**
  * \brief Description.
  */
  struct InnerStruct{};
  
private:
  static int m_staticVariable;
  
private:
  int m_variable = 0;
  
private:
  void privateFunction();
  
private:
  privateConstructor();
  ~privateDestructor();
  
public:
  /**
  * \brief description.
  */
  publicConstructor();
  
  /**
  * \brief description.
  */
  ~publicDestructor();
  
public:
  /**
  * \brief description.
  */
  void public1Function();
  
  /**
  * \brief description.
  */
  void public2Function();
  
public:
  /**
  * \brief description.
  */
  static void staticFunction();
  
public:
  /**
  * \brief description.
  */
  PROPERTY(float, someVariable) = 0.0f;
  
public:
  /**
  * \brief description.
  */
  STATIC_PROPERTY(float, someStaticVariable);
}

```

# Style and naming used while developing source code in C#

Template for a default source code file
```CSharp
// ReCrafted © 2016-2017 Always Too Late

/// <summary>
/// [DESCRIPTION]
/// </summary>
[ACCESS] class [NAME] : [EXTENDS]
{
   [MEMBERS]
}
```

# 1. Comments
We use only one-line comments (except if they are used to disable some part of code), in which there are spaces after the beginning and text starts with lowercase.
Commas and dots are allowed to use.
```CSharp
// this is a comment,
// which is pretty cool.
```
Tripple-slash(XML docs) comments should look like this:
```CSharp
/// <summary>
/// Some summary.
/// </summary>
/// <*>
/// some commment,
/// end.
/// <*>
/// ...
---
```

# 2. Variables
Variables ALWAYS start with accessibility declaration ('private/public etc.')
Keywords order for static variables: 'public/private static (readonly)'.
Also variables should often be transformed into property with proper setter accessor declaration.

```CSharp
public string Variable { get; private set; }
public static string Variable { get; private set; }
```

Each and every public/protected/internal variable should have its own description, which is used in API.
```CSharp
/// <summary>
/// This is a `Variable` description.
/// </summary>
public string Variable { get; private set; }
```

- Private variables' names start with underscore ('_') and lowercase.
```CSharp
private string _variable;
```

- Public/protected variables' names always start with uppercase.
```CSharp
public string Variable;
```

- Public static variables' names always start with uppercase.
```CSharp
public static string Variable;
```

- * static readonly variables' names always start with uppercase.
```CSharp
private static readonly string Variable;
```

- Properties' names always start with uppercase.
```CSharp
private string Variable { get; set; }
```

# 3. Classes / Structs / Interfaces
Classes' names start with uppercase and should have description in XML comments (if they're public/protected/internal).

```CSharp
/// <summary>
/// This is class description.
/// </summary>
internal class SomeClass
{
   // ...
}
```

Interfaces always start with big 'i' letter, also first letter is uppercase.
```CSharp
internal interface IInterfaceName
{
   // ...
}
```

# 4. Methods
Name of every method starts with uppercase and they should have description (with all the parameters) in XML comments (if they're public/protected/internal).
Private Methods do not require any comment.

```CSharp
/// <summary>
/// This is method description.
/// </summary>
private void SomeMethod() 
{
   // ..
}
```

```CSharp
/// <summary>
/// Some summary.
/// </summary>
/// <param name="x">Some param</param>
public void SomeMethod(object x)
{
   // ..
}

private void SomeMethod()
{
   // ..
}
```

# 5. Property
TODO

# 6. Delegates/Actions
TODO

# 7. Layout/Members' order in source code files
Layout for all source code files:

```CSharp
// ReCrafted © 2016-2017 Always Too Late

using System;

namespace NAMESPACE
{
  /// <summary>
  /// Some summary.
  /// </summary>
  access modifier Name
  {
    /// <summary>
    /// Some summary.
    /// </summary>
    access modifier InnerStruct {}
    
    private static readonly SomeVariable;
    private static _someVariable;
    private int _someVariable;
    
    // internal ones...
    
    /// <summary>
    /// Some summary.
    /// </summary>
    public int SomeVariable;
    
    /// <summary>
    /// Some summary.
    /// </summary>
    public PublicCtor() {}
    
    /// <summary>
    /// Some summary.
    /// </summary>
    public ~PublicDtor() {}
    
    // internal ones...
    
    private PrivateCtor() {}
    
    private ~PrivateDtor() {}
    
    /// <summary>
    /// Some summary.
    /// </summary>
    override SomeFunction() {}
    
    /// <summary>
    /// Some summary.
    /// </summary>
    public SomeFunction() {}
    
    // internal ones...
    
    private SomeFunction() {}
    
    /// <summary>
    /// Some summary.
    /// </summary>
    public static SomeFunction() {}
    
    /// <summary>
    /// Some summary.
    /// </summary>
    internal static SomeFunction() {}
    
    private static SomeFunction() {}
    
    /// <summary>
    /// Some summary.
    /// </summary>
    public int SomeProperty { get; set; }
    
    /// <summary>
    /// Some summary.
    /// </summary>
    public int SomeProperty { get; private set; }
    
    /// <summary>
    /// Some summary.
    /// </summary>
    public int SomeProperty { get; protected set; }
    
    // internal ones...
    
    /// <summary>
    /// Some summary.
    /// </summary>
    public static int SomeProperty { get; set; }
    
    /// <summary>
    /// Some summary.
    /// </summary>
    public static int SomeProperty { get; private set; }
    
    /// <summary>
    /// Some summary.
    /// </summary>
    public static int SomeProperty { get; protected set; }
    
    // internal ones...
  }
}
```
