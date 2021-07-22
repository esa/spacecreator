#pragma once

#include <QByteArray>

namespace helpers {

static inline QByteArray emptyDoc()
{
    return QByteArray("<DeploymentView>\n</DeploymentView>");
}

static inline QByteArray nonValidDoc()
{
    return QByteArray("<InterfaceView>\n</InterfaceView>");
}

static inline QByteArray singleNode()
{
    return QByteArray("<DeploymentView>"
                      "    <Node name=\"node_linux\"></Node>"
                      "</DeploymentView>");
}

static inline QByteArray singleNodeWithProperty()
{
    return QByteArray("<DeploymentView>"
                      "    <Node name=\"node_linux\">"
                      "         <Property name=\"Taste::coordinates\" value=\" 54900 69600 62900 77600 \"/>"
                      "    </Node>"
                      "</DeploymentView>");
}

static inline QByteArray unknownProperty()
{
    return QByteArray("<DeploymentView>"
                      "    <Unknown name=\"some_text\"></Unknown>"
                      "</DeploymentView>");
}

static inline QByteArray openCloseTagMismatched()
{
    return QByteArray("<DeploymentView>"
                      "    <Unknown name=\"some_text\"></Test>"
                      "</DeploymentView>");
}

static inline QByteArray malformedXml()
{
    return QByteArray("<DeploymentView>"
                      "    <Unknown name=\"some_text\"><Test>"
                      "    <Test2 />"
                      "<DeploymentView>");
}

static inline QByteArray singlePartitionWithNode()
{
    return QByteArray(
            "<DeploymentView>"
            "    <Node name=\"node_linux\">"
            "        <Partition ada_runtime=\"\""
            "             coverage=\"FALSE\" cpu=\"x86_linux\" cpu_classifier=\"ocarina_processors_x86::x86.linux\""
            "             cpu_platform=\"PLATFORM_NATIVE\" name=\"demo\" package=\"ocarina_processors_x86\""
            "             vp_classifier=\"\" vp_name=\"\" vp_platform=\"PLATFORM_NONE\">"
            "        </Partition>"
            "    </Node>"
            "</DeploymentView>");
}

static inline QByteArray singlePartitionOnly()
{
    return QByteArray(
            "<DeploymentView>"
            "        <Partition ada_runtime=\"\""
            "             coverage=\"FALSE\" cpu=\"x86_linux\" cpu_classifier=\"ocarina_processors_x86::x86.linux\""
            "             cpu_platform=\"PLATFORM_NATIVE\" name=\"demo\" package=\"ocarina_processors_x86\""
            "             vp_classifier=\"\" vp_name=\"\" vp_platform=\"PLATFORM_NONE\">"
            "        </Partition>"
            "</DeploymentView>");
}

static inline QByteArray singlePartitionWithFunctions()
{
    return QByteArray(
            "<DeploymentView>"
            "        <Partition ada_runtime=\"\""
            "             coverage=\"FALSE\" cpu=\"x86_linux\" cpu_classifier=\"ocarina_processors_x86::x86.linux\""
            "             cpu_platform=\"PLATFORM_NATIVE\" name=\"demo\" package=\"ocarina_processors_x86\""
            "             vp_classifier=\"\" vp_name=\"\" vp_platform=\"PLATFORM_NONE\">"
            "                 <function>asw_gnc_con</function>"
            "                 <function>asw_gnc_gui</function>"
            "                 <function>asw_gnc_nav</function>"
            "                 <function>gnc_con</function>"
            "                 <function>gnc_drs</function>"
            "                 <function>gnc_fm</function>"
            "                 <function>gnc_gui</function>"
            "                 <function>gnc_nav</function>"
            "                 <function>sl_dev_external</function>"
            "                 <function>sl_svc_datapool</function>"
            "        </Partition>"
            "</DeploymentView>");
}

static inline QByteArray singleDeviceWithNode()
{
    return QByteArray("<DeploymentView>"
                      "    <Node name=\"node_linux\">"
                      "        <Device name=\"ethernet\""
                      "             proc=\"p1\" bus=\"eth0\" classifier=\"ocarina_processors_x86::x86.linux\""
                      "             port=\"link\">"
                      "        </Device>"
                      "    </Node>"
                      "</DeploymentView>");
}

static inline QByteArray singleDeviceOnly()
{
    return QByteArray("<DeploymentView>"
                      "        <Device name=\"ethernet\""
                      "             proc=\"p1\" bus=\"eth0\" classifier=\"ocarina_processors_x86::x86.linux\""
                      "             port=\"link\">"
                      "        </Device>"
                      "</DeploymentView>");
}

static inline QByteArray singleNodeWithPartitionAndDevice()
{
    return QByteArray(
            "<DeploymentView>"
            "    <Node name=\"node_linux\">"
            "        <Partition ada_runtime=\"\""
            "             coverage=\"FALSE\" cpu=\"x86_linux\" cpu_classifier=\"ocarina_processors_x86::x86.linux\""
            "             cpu_platform=\"PLATFORM_NATIVE\" name=\"demo\" package=\"ocarina_processors_x86\""
            "             vp_classifier=\"\" vp_name=\"\" vp_platform=\"PLATFORM_NONE\">"
            "        </Partition>"
            "        <Device name=\"ethernet\""
            "             proc=\"p1\" bus=\"eth0\" classifier=\"ocarina_processors_x86::x86.linux\""
            "             port=\"link\">"
            "        </Device>"
            "    </Node>"
            "</DeploymentView>");
}

static inline QByteArray twoConnections()
{
    return QByteArray(
            "<DeploymentView>"
            "        <Connection from_node=\"B\" from_port=\"A_pong\" to_bus=\"eth0\" to_node=\"A\" to_port=\"ping\" />"
            "        <Connection from_node=\"A\" from_port=\"B_pong\" to_bus=\"eth0\" to_node=\"B\" to_port=\"ping\" />"
            "</DeploymentView>");
}

static inline QByteArray twoConnectionsAndNode()
{
    return QByteArray(
            "<DeploymentView>"
            "    <Node name=\"node_linux\"></Node>"
            "    <Connection from_node=\"B\" from_port=\"A_pong\" to_bus=\"eth0\" to_node=\"A\" to_port=\"ping\" />"
            "    <Connection from_node=\"A\" from_port=\"B_pong\" to_bus=\"eth0\" to_node=\"B\" to_port=\"ping\" />"
            "</DeploymentView>");
}

static inline QByteArray twoConnectionsAndNodeWithPartitionAndDevice()
{
    return QByteArray(
            "<DeploymentView>"
            "    <Node name=\"node_linux\">"
            "        <Partition ada_runtime=\"\""
            "             coverage=\"FALSE\" cpu=\"x86_linux\" cpu_classifier=\"ocarina_processors_x86::x86.linux\""
            "             cpu_platform=\"PLATFORM_NATIVE\" name=\"demo\" package=\"ocarina_processors_x86\""
            "             vp_classifier=\"\" vp_name=\"\" vp_platform=\"PLATFORM_NONE\">"
            "        </Partition>"
            "        <Device name=\"ethernet\""
            "             proc=\"p1\" bus=\"eth0\" classifier=\"ocarina_processors_x86::x86.linux\""
            "             port=\"link\">"
            "        </Device>"
            "    </Node>"
            "    <Connection from_node=\"B\" from_port=\"A_pong\" to_bus=\"eth0\" to_node=\"A\" to_port=\"ping\" />"
            "    <Connection from_node=\"A\" from_port=\"B_pong\" to_bus=\"eth0\" to_node=\"B\" to_port=\"ping\" />"
            "</DeploymentView>");
}

static inline QByteArray twoConnectionsAndNodeWithDeviceAndPartitionWithFunctions()
{
    return QByteArray(
            "<DeploymentView>"
            "    <Node name=\"node_linux\">"
            "        <Partition ada_runtime=\"\""
            "             coverage=\"FALSE\" cpu=\"x86_linux\" cpu_classifier=\"ocarina_processors_x86::x86.linux\""
            "             cpu_platform=\"PLATFORM_NATIVE\" name=\"demo\" package=\"ocarina_processors_x86\""
            "             vp_classifier=\"\" vp_name=\"\" vp_platform=\"PLATFORM_NONE\">"
            "                 <function>asw_gnc_con</function>"
            "                 <function>asw_gnc_gui</function>"
            "                 <function>asw_gnc_nav</function>"
            "                 <function>gnc_con</function>"
            "                 <function>gnc_drs</function>"
            "                 <function>gnc_fm</function>"
            "                 <function>gnc_gui</function>"
            "                 <function>gnc_nav</function>"
            "                 <function>sl_dev_external</function>"
            "                 <function>sl_svc_datapool</function>"
            "        </Partition>"
            "        <Device name=\"ethernet\""
            "             proc=\"p1\" bus=\"eth0\" classifier=\"ocarina_processors_x86::x86.linux\""
            "             port=\"link\">"
            "        </Device>"
            "    </Node>"
            "    <Connection from_node=\"B\" from_port=\"A_pong\" to_bus=\"eth0\" to_node=\"A\" to_port=\"ping\" />"
            "    <Connection from_node=\"A\" from_port=\"B_pong\" to_bus=\"eth0\" to_node=\"B\" to_port=\"ping\" />"
            "</DeploymentView>");
}

static inline QByteArray singleMessage()
{
    return QByteArray(
            "<DeploymentView>"
            "    <Node name=\"node_linux\">"
            "        <Partition ada_runtime=\"\""
            "             coverage=\"FALSE\" cpu=\"x86_linux\" cpu_classifier=\"ocarina_processors_x86::x86.linux\""
            "             cpu_platform=\"PLATFORM_NATIVE\" name=\"demo\" package=\"ocarina_processors_x86\""
            "             vp_classifier=\"\" vp_name=\"\" vp_platform=\"PLATFORM_NONE\">"
            "                 <function>gnc_fm</function>"
            "        </Partition>"
            "        <Device requires_bus_access=\"ocarina_buses::ip.i\" port=\"eth0\" "
            "             asn1file=\"/opt/ip.asn\" asn1type=\"IP-Conf-T\" asn1module=\"POHICDRIVER-IP\" />"
            "    </Node>"
            "    <Node name=\"node_bsd\">"
            "        <Partition ada_runtime=\"\""
            "             coverage=\"FALSE\" cpu=\"x86_linux\" cpu_classifier=\"ocarina_processors_x86::x86.linux\""
            "             cpu_platform=\"PLATFORM_NATIVE\" name=\"demo\" package=\"ocarina_processors_x86\""
            "             vp_classifier=\"\" vp_name=\"\" vp_platform=\"PLATFORM_NONE\">"
            "                 <function>gnc_gui</function>"
            "        </Partition>"
            "        <Device requires_bus_access=\"ocarina_buses::ip.i\" port=\"eth0\" "
            "             asn1file=\"/opt/ip.asn\" asn1type=\"IP-Conf-T\" asn1module=\"POHICDRIVER-IP\" />"
            "    </Node>"
            "    <Connection name=\"\" from_node=\"node_linux\" from_port=\"eth0\" to_bus=\"ocarina_buses::ip.i\""
            "         to_node=\"node_bsd\" to_port=\"eth0\">"
            "        <Message name=\"init\" from_function=\"gnc_fm\" to_function=\"gnc_gui\" />"
            "    </Connection>"
            "</DeploymentView>");
}

} // namespace helpers
