#line 1 "/Volumes/src/usd-rs/usd-cpp/thirdparty/USD/pxr/usd/sdr/shaderProperty.h"
//
// Copyright 2018 Pixar
//
// Licensed under the Apache License, Version 2.0 (the "Apache License")
// with the following modification; you may not use this file except in
// compliance with the Apache License and the following modification to it:
// Section 6. Trademarks. is deleted and replaced with:
//
// 6. Trademarks. This License does not grant permission to use the trade
//    names, trademarks, service marks, or product names of the Licensor
//    and its affiliates, except as required to comply with Section 4(c) of
//    the License and to reproduce the content of the NOTICE file.
//
// You may obtain a copy of the Apache License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the Apache License with the above modification is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied. See the Apache License for the specific
// language governing permissions and limitations under the Apache License.
//

#ifndef PXR_USD_SDR_SHADER_PROPERTY_H
#define PXR_USD_SDR_SHADER_PROPERTY_H

/// \file sdr/shaderProperty.h

#include "pxr/pxr.h"
#include "pxr/usd/sdr/api.h"
#include "pxr/base/tf/staticTokens.h"
#include "pxr/base/tf/token.h"
#include "pxr/base/tf/weakBase.h"
#include "pxr/base/vt/value.h"
#include "pxr/usd/ndr/property.h"
#include "pxr/usd/sdr/shaderNode.h"

PXR_NAMESPACE_OPEN_SCOPE

// If additional types are added here, it's also worth trying to add a mapping
// to the equivalent Sdf type in the implementation file.
#define SDR_PROPERTY_TYPE_TOKENS \
    ((Int,      "int"))          \
    ((String,   "string"))       \
    ((Float,    "float"))        \
    ((Color,    "color"))        \
    ((Point,    "point"))        \
    ((Normal,   "normal"))       \
    ((Vector,   "vector"))       \
    ((Matrix,   "matrix"))       \
    ((Struct,   "struct"))       \
    ((Terminal, "terminal"))     \
    ((Vstruct,  "vstruct"))      \
    ((Unknown,  "unknown"))

// Note: Metadata keys that are generated by parsers should start with
// "__SDR__" to reduce the risk of collision with metadata actually in the
// shader.
#define SDR_PROPERTY_METADATA_TOKENS                 \
    ((Label, "label"))                               \
    ((Help, "help"))                                 \
    ((Page, "page"))                                 \
    ((RenderType, "renderType"))                     \
    ((Role, "role"))                                 \
    ((Widget, "widget"))                             \
    ((Hints, "hints"))                               \
    ((Options, "options"))                           \
    ((IsDynamicArray, "isDynamicArray"))             \
    ((Connectable, "connectable"))                   \
    ((ValidConnectionTypes, "validConnectionTypes")) \
    ((VstructMemberOf, "vstructMemberOf"))           \
    ((VstructMemberName, "vstructMemberName"))       \
    ((VstructConditionalExpr, "vstructConditionalExpr"))\
    ((IsAssetIdentifier, "__SDR__isAssetIdentifier"))\
    ((ImplementationName, "__SDR__implementationName"))\
    ((DefaultInput, "__SDR__defaultinput"))          \
    ((Target, "__SDR__target"))                      \
    ((Colorspace, "__SDR__colorspace"))


// The following tokens are valid values for the metadata "role"
#define SDR_PROPERTY_ROLE_TOKENS \
    ((None, "none"))

TF_DECLARE_PUBLIC_TOKENS(SdrPropertyTypes, SDR_API, SDR_PROPERTY_TYPE_TOKENS);
TF_DECLARE_PUBLIC_TOKENS(SdrPropertyMetadata, SDR_API, 
                         SDR_PROPERTY_METADATA_TOKENS);
TF_DECLARE_PUBLIC_TOKENS(SdrPropertyRole, SDR_API,
                         SDR_PROPERTY_ROLE_TOKENS);

/// \class SdrShaderProperty
///
/// A specialized version of `NdrProperty` which holds shading information.
///
class SdrShaderProperty : public NdrProperty
{
public:
    // Constructor.
    SDR_API
    SdrShaderProperty(
        const TfToken& name,
        const TfToken& type,
        const VtValue& defaultValue,
        bool isOutput,
        size_t arraySize,
        const NdrTokenMap& metadata,
        const NdrTokenMap& hints,
        const NdrOptionVec& options
    );

    /// \name Metadata
    /// The metadata returned here is a direct result of what the parser plugin
    /// is able to determine about the property. See the documentation for a
    /// specific parser plugin to get help on what the parser is looking for to
    /// populate these values.
    /// @{

    /// The label assigned to this property, if any. Distinct from the name
    /// returned from `GetName()`. In the context of a UI, the label value
    /// might be used as the display name for the property instead of the name.
    SDR_API
    const TfToken& GetLabel() const { return _label; }

    /// The help message assigned to this property, if any.
    SDR_API
    const std::string& GetHelp() const;

    /// The page (group), eg "Advanced", this property appears on, if any.
    SDR_API
    const TfToken& GetPage() const { return _page; }

    /// The widget "hint" that indicates the widget that can best display the
    /// type of data contained in this property, if any. Examples of this value
    /// could include "number", "slider", etc.
    SDR_API
    const TfToken& GetWidget() const { return _widget; }

    /// Any UI "hints" that are associated with this property. "Hints" are
    /// simple key/value pairs.
    SDR_API
    const NdrTokenMap& GetHints() const { return _hints; }

    /// If the property has a set of valid values that are pre-determined, this
    /// will return the valid option names and corresponding string values (if
    /// the option was specified with a value).
    SDR_API
    const NdrOptionVec& GetOptions() const { return _options; }

    /// Returns the implementation name of this property.  The name of the
    /// property is how to refer to the property in shader networks.  The
    /// label is how to present this property to users.  The implementation
    /// name is the name of the parameter this property represents in the
    /// implementation.  Any client using the implementation \b must call
    /// this method to get the correct name;  using \c getName() is not
    /// correct.
    SDR_API
    const std::string& GetImplementationName() const;

    /// @}


    /// \name VStruct Information
    /// @{

    /// If this field is part of a vstruct, this is the name of the struct.
    SDR_API
    const TfToken& GetVStructMemberOf() const {
        return _vstructMemberOf;
    }

    /// If this field is part of a vstruct, this is its name in the struct.
    SDR_API
    const TfToken& GetVStructMemberName() const {
        return _vstructMemberName;
    }

    /// Returns true if this field is part of a vstruct.
    SDR_API
    bool IsVStructMember() const;

    /// Returns true if the field is the head of a vstruct.
    SDR_API
    bool IsVStruct() const;


    /// If this field is part of a vstruct, this is the conditional expression
    SDR_API
    const TfToken& GetVStructConditionalExpr() const {
        return _vstructConditionalExpr;
    }

    /// @}


    /// \name Connection Information
    /// @{

    /// Whether this property can be connected to other properties. If this
    /// returns `true`, connectability to a specific property can be tested via
    /// `CanConnectTo()`.
    SDR_API
    bool IsConnectable() const override { return _isConnectable; }

    /// Gets the list of valid connection types for this property. This value
    /// comes from shader metadata, and may not be specified. The value from
    /// `NdrProperty::GetType()` can be used as a fallback, or you can use the
    /// connectability test in `CanConnectTo()`.
    SDR_API
    const NdrTokenVec& GetValidConnectionTypes() const {
        return _validConnectionTypes;
    }

    /// Determines if this property can be connected to the specified property.
    SDR_API
    bool CanConnectTo(const NdrProperty& other) const override;

    /// @}


    /// \name Utilities
    /// @{

    /// Converts the property's type from `GetType()` into a `SdfValueTypeName`.
    ///
    /// Two scenarios can result: an exact mapping from property type to Sdf
    /// type, and an inexact mapping. In the first scenario, the first element
    /// in the pair will be the cleanly-mapped Sdf type, and the second element,
    /// a TfToken, will be empty. In the second scenario, the Sdf type will be
    /// set to `Token` to indicate an unclean mapping, and the second element
    /// will be set to the original type returned by `GetType()`.
    SDR_API
    const SdfTypeIndicator GetTypeAsSdfType() const override;

    /// Determines if the value held by this property is an asset identifier
    /// (eg, a file path); the logic for this is left up to the parser.
    ///
    /// Note: The type returned from `GetTypeAsSdfType()` will be `Asset` if
    /// this method returns `true` (even though its true underlying data type
    /// is string).
    SDR_API
    bool IsAssetIdentifier() const;

    /// Determines if the value held by this property is the default input 
    /// for this node.
    SDR_API
    bool IsDefaultInput() const;

    /// @}

protected:
    SdrShaderProperty& operator=(const SdrShaderProperty&) = delete;

    // Allow the shader's post process function to access the property's
    // internals. Some property information can only be determined after parse
    // time.
    friend void SdrShaderNode::_PostProcessProperties();

    // Some metadata values cannot be returned by reference from the main
    // metadata dictionary because they need additional parsing.
    const NdrTokenMap _hints;
    const NdrOptionVec _options;

    // Tokenized metadata
    NdrTokenVec _validConnectionTypes;
    TfToken _label;
    TfToken _page;
    TfToken _widget;
    TfToken _vstructMemberOf;
    TfToken _vstructMemberName;
    TfToken _vstructConditionalExpr;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif // PXR_USD_SDR_SHADER_PROPERTY_H
