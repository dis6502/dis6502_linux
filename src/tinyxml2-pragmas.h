
#pragma warning( disable : 26401 ) // Do not delete a raw pointer that is not an owner
#pragma warning( disable : 26409 ) // Avoid calling new and delete explicitly, use std::make_unique<T> instead (r.11).
#pragma warning( disable : 26429 ) // Symbol 'a' is never tested for nullness, it can be marked as not_null(f.23).dis6502
#pragma warning( disable : 26432 ) // If you define or delete any default operation in the type 'class tinyxml2::MemPoolT<56>', define or delete them all (c.21).
#pragma warning( disable : 26433 ) // Function 'tinyxml2::XMLText::ToText' should be marked with 'override' 
#pragma warning( disable : 26433 ) // Function 'tinyxml2::XMLText::ToText' should be marked with 'override' 
#pragma warning( disable : 26436 ) // The type 'class tinyxml2::XMLUnknown' with a virtual function needs either public virtual or protected non - virtual destructor(c.35).dis6502
#pragma warning( disable : 26465 ) // Don't use const_cast to cast away const or volatile. const_cast is not required; constness or volatility is not being removed by this conversion (type.3).
#pragma warning( disable : 26481 ) // Don't use pointer arithmetic. Use span instead (bounds.1).
#pragma warning( disable : 26492 ) // Don't use const_cast to cast away const or volatile (type.3).
#pragma warning( disable : 26400 ) // Do not assign the result of an allocation or a function call with an owner<T> return value to a raw pointer, use owner<T> instead(i.11).
#pragma warning( disable : 26447 ) // The function is declared 'noexcept' but calls function 'PopDepth()' which may throw exceptions(f.6).
#pragma warning( disable : 26455 ) // Default constructor should not throw.Declare it 'noexcept' (f.6).
#pragma warning( disable : 26461 ) // The pointer argument 'p' for function 'tinyxml2::XMLUtil::SkipWhiteSpace' can be marked as a pointer to const (con.3)
#pragma warning( disable : 26472 ) // Don't use a static_cast for arithmetic conversions. Use brace initialization, gsl::narrow_cast or gsl::narrow (type.1).
#pragma warning( disable : 26485 ) // Expression 'this->_pool' : No array to pointer decay(bounds.3).
#pragma warning( disable : 26497 ) // You can attempt to make 'tinyxml2::XMLUtil::IsUTF8Continuation' constexpr unless it contains any undefined behavior(f.4).
#pragma warning( disable : 26812 ) // The enum type 'tinyxml2::XMLElement::ElementClosingType' is unscoped.Prefer 'enum class' over 'enum' (Enum.3).
#pragma warning( disable : 26812 ) // The enum type 'tinyxml2::XMLError' is unscoped.Prefer 'enum class' over 'enum' (Enum.3).
#pragma warning( disable : 26814 ) // The const variable 'TIXML2_PATCH_VERSION' can be computed at compile - time.Consider using constexpr (con.5).


