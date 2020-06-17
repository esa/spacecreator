@mainpage

# Installation

For installation etc. read the README.md in the main dirrectory of the repository.


# Tools

* C++14 compatible compiler
* Qt >5.9
* Grantlee (for saving files) https://github.com/steveire/grantlee
* Antlr (v4) for parsing files http://www.antlr.org/
* CMake (build/project tool)
* clang-format is used to format the C++ files. The configuration is stored in _clang-format
* Doxygen is used to create the main and the API documentation
* asn1scc to transform .asn into .xml for reading .asn files https://github.com/ttsiodras/asn1scc


# Basic informations about the editor

Space Creator is part of the ESA [TASTE](https://taste.tools/) project.


## ASN.1 editor

The ASN.1 editor is implemented as a pure QtCreator plugin. It features

* Code highlighting
* Code completion
* ASN.1 Types View
* And more

For reading .asn files, asn1scc is used (transforming into XML).

## AADL editor

[AADL Wiki Page](https://en.wikipedia.org/wiki/Architecture_Analysis_%26_Design_Language).

## MSC editor

MSC editor has 2 major parts.

One is for editing the document hierarchy in the msc file. Every document can either have one or more (depending on the type) sub documents. Documents of type "leaf" contain MSC charts.
The type of the document is stored as comment of that document.

The second part is for editing a MSC chart. One important aspect of the editing are the message parameters. There ASN.1 is used for the parameters.
Another aspect is, that the layout is not purely automatic. When moving entities (instances, messages, ...) the geometry information is stored as comment in CIF format. Where the geometry unit is mm (dpi of the monitor is used).

One special mode exists for the editor. The "MSC Remote Handler" or "Streaming" mode. There a Websocket interface is used to edit a MSC chart. Information about the msc remote handling can be found in [MSC Remote Handler](@ref RemotehandlerPage).

To learn more about MSC, see [MSC Wiki Page](https://en.wikipedia.org/wiki/Message_sequence_chart) and the official specification http://www.itu.int/rec/T-REC-Z.120.

Note: the MSC parser is not 100% able to parse all specified documents. On the other hand is able to load older spec versions and some additions (for example message parameters). Those changes are documented in the Msc.g4 file.


### MSC Conversion Template

MSC editor can be used to convert msc files. Fro more information see [MSC Conversion Template](@ref MSCConversion).

# Internal Architecture

Details on the architecture of is described in [Architecture documentation](@ref ArchitecturePage)
