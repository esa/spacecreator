# MSC Conversion Template {#MSCConversion}

[TOC]

## General

The MSC editor can be used to convert msc files. The command parameter are the following:

`./msceditor --open-msc <msc-file> --template <template-root-file> --export <output-file>`

The template system used to save the msc file is Grantlee https://github.com/steveire/grantlee.
Grantlee uses the syntax of the Django template system (https://docs.djangoproject.com/en/3.0/topics/templates/ and http://www.grantlee.org/apidox/differences_django.html).
The Q_PROPERTY of the MSC objects can be used as variables. If a property is an enum, then the usage is described in http://www.grantlee.org/apidox/for_app_dev.html.

Have a look at _src/libs/msccore/mscresources_ to see the template used to save in msc format.

### XML example

A start of an example of an template to export it to xml is here:

```
{% for document in model.documents %}
<Document name="{{ document.name }}">
    {% if document.commentString %}
    <Comment>
        {{ document.commentString }}
    </Comment
    {% endif %}
    {% for chart in document.charts %}
    <Chart name="{{ chart.name }}">
        {% include "xml_chart_body.tmplt" %}
    </Chart>
    {% endfor %}
</Document>
{% endfor %}
```


## Model

The [Model](@ref msc::MscModel) is the root object that is used for the export. Normally a model has one root document.
But the standard allows to have more than one, and even have chart(s) only.

Relevant properties are:
* **dataLanguage** The file format of the _dataDefinitionString_. Usually that is empty or ASN1.
* **dataDefinitionString** The filename of the data defintion (the ASN.1 file).
* **documents** A list of all root [documents](@ref msc::MscDocument). Usually there is only one.
* **charts** A list of all root [charts](@ref msc::MscChart). Usually there is no chart.

## Document

A [document](@ref msc::MscDocument) contains either documents or charts.

Relevant properties are:
* **hierarchyTypeString** The hierarchy type is an extension of msc. It indicates the relation of it's child documents (LEAF, AND, OR, PARALLEL, ...).
* **documents** Child documents.
* **charts** Child [charts](@ref msc::MscChart).
* **declarations** A list of [message declarations](@ref msc::MscMessageDeclaration).

## Chart

[document](@ref msc::MscChart) contains the  MSC chart.

Relevant properties are:
* **name** Name.
* **commentString** The comment as plain string.
* **instances** A list of the [instances](@ref msc::MscInstance).
* **instanceEvents** A ordered (by time of occurence) list of of events. Events are action, condition, coregion, create, message and timer.

## Instance

[Instance](@ref msc::MscInstance) an instance in a chart.

Relevant properties are:
* **name** Name.
* **commentString** The comment as plain string.
* **kind** If empty, the name is the kind. As defined by MSC.
* **denominator** The denominator of the kind.
* **denominatorAndKind** denominator and kind joined with one separating space.
* **inheritance** The instance kind this instances inherits from.
* **explicitStop** Bool, that if true, the instance stops after the last instance event linked to this instance.

## Action

[Action](@ref msc::MscAction) an action in a chart.

Relevant properties are:
* **name** Name.
* **commentString** The comment as plain string.
* **entityType** Enum telling what event type it is. Values (for events) are _Message_, _Timer_, _Condition_, _Action_, _Coregion_, _Create_.
* **informalAction** Text of an informal action.
* **instance** The [instance](@ref msc::MscInstance) this action belongs to.

## Condition

[Condition](@ref msc::MscCondition) a condition in a chart.

Relevant properties are:
* **name** Name/text of the condition.
* **commentString** The comment as plain string.
* **entityType** Enum telling what event type it is. Values (for events) are _Message_, _Timer_, _Condition_, _Action_, _Coregion_, _Create_.
* **instance** The [instance](@ref msc::MscInstance) this condition belongs to.
* **shared** Bool that if true, then de condition is shared by all instances.

## Coregion

[Coregion](@ref msc::MscCoregion) a coregion in a chart.

Relevant properties are:
* **name** Name/text of the Coregion.
* **commentString** The comment as plain string.
* **entityType** Enum telling what event type it is. Values (for events) are _Message_, _Timer_, _Condition_, _Action_, _Coregion_, _Create_.
* **instance** The [instance](@ref msc::MscInstance) this coregion belongs to.
* **type** Enum that can be either _Begin_ or _End_.

## Create

[Create](@ref msc::MscCreate) a create message in a chart.

Relevant properties are:

Same as a message. The _targetInstance_ is the one that is created by this message.

## Message

[Message](@ref msc::MscMessage) a message in a chart.

Relevant properties are:
* **name** Name of the message.
* **commentString** The comment as plain string.
* **entityType** Enum telling what event type it is. Values (for events) are _Message_, _Timer_, _Condition_, _Action_, _Coregion_, _Create_.
* **messageInstanceName** If the name, input and output are not unique, a message instance name is set.
* **sourceInstance** [instance](@ref msc::MscInstance) the messages starts at. If empty (nullptr), it's from the environment.
* **targetInstance** [instance](@ref msc::MscInstance) the messages points to. If empty (nullptr), it's to the environment.
* **parameterString** All parameters as one string.

## Timer

[Timer](@ref msc::MscTimer) a timer in a chart.

Relevant properties are:
* **name** Name/text of the timer.
* **commentString** The comment as plain string.
* **entityType** Enum telling what event type it is. Values (for events) are _Message_, _Timer_, _Condition_, _Action_, _Coregion_, _Create_.
* **timerType** Enum that can be either _Start_, _Stop_ or _Timeout_.
* **instance** The [instance](@ref msc::MscInstance) this coregion belongs to.
* **timerInstanceName** Where the timer's name is not sufficient for a unique mapping, the timerInstanceName is set.

## DataStatement

[DataStatement](@ref msc::DataStatement) a datastatement can be listed in a document.

Properties:
* **type** Enum that can be either _Define_, _Undefine_ or _Binding_.
* **variableString** Text of the statement
