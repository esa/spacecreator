namespace seds::test {

class tst_EtbSrsRequirements
{
public:
    /**
     * Date: 2022.12.09
     * Result: all delivered documentation (SUM, ADD, SRS, AB, EDDFR, ESR, FR, SR and SVSR (this document) use English.)
     *
     * \SRS  ETB-GEN-10
     */
    auto test_ProjectDocumentationLanguage() {}

    /**
     * Date: 2022.12.09
     * Result: ETB-N7S-SVSR-001-RD5 lists several tests demonstrating the capability to generate ASN.1/ACN, interfaceview.xml and
     * SDL files used to store data, architecture and behavior models within a SpaceCreator project. SpaceCreator uses
     * XML for architecture model storage instead of AADL. The AADL, used by other parts of the TASTE toolchain,
     * can be generated from the XML files via already existing TASTE functionality. The SDL files generated by STBSW
     * contain only SDL process description. The required system structure definition (including channels, inputs and
     * outputs) can be generated by TASTE from AADL generated from XML generated from SEDS. Integration of the generated
     * artefacts requires additional manual actions before a compilation of a TASTE project.
     *
     * \SRS  ETB-OPER-40
     */
    auto test_ImportModelsGeneratedFromSedsToSpaceCreator() {}

    /**
     * Date: 2022.12.09
     * Result: ETB-N7S-SVSR-001-RD5 lists several tests demonstrating the capability to generate SEDS models from ASN.1/ACN files,
     * as well as interfaceview.xml used to store data and architecture models within a SpaceCreator project.
     *
     * \SRS  ETB-OPER-50
     */
    auto test_ExportSedsModelsFromSpaceCreator() {}

    /**
     * Date: 2022.12.09
     * Result: TASTE 10 is the development and testing environment of STBSW, and therefore STBSW can be run on TASTE 10.
     *
     * \SRS  ETB-RES-10
     */
    auto test_RunSedsImporterExporterOnTaste10() {}

    /**
     * EDS Device Driver Feasibility Report is provided in the ETB-N7S-TN-002 document.
     *
     * \SRS ETB-DES-70
     */
    auto test_ProvideSedsDriversSuitabilityTechnicalNote() {}

    /**
     * As per chapter 4.1 of ETB-N7S-ADD-001, STBSW GUI is exposed via SedsPlugin component, integrated with SpaceCreatorPlugin
     * and QtCreator hosting the SpaceCreator environment.
     *
     * \SRS  ETB-DES-140
     */
    auto test_SedsTasteBridgeGuiIntegratedInSpaceCreator() {}

    /**
     * As per chapter 4.1 of ETB-N7S-ADD-001, STBSW is completely integrated with the QtCreator-based SpaceCreator software, which
     * is a part of TASTE. It is written using C++. The only additionally introduced libraries are toml++ and libxml2,
     * which do not preclude software use on Windows 10. Re-used asn1library (which is already a part of SpaceCreator)
     * relies on ASN1SCC, which can be executed on Windows 10. Therefore, the choice of technologies for STBSW
     * implementation does not introduce any additional platform limitations in additions to the ones already present
     * in the TASTE software.
     *
     * \SRS  ETB-POR-10
     */
    auto test_SedsTasteBridgeIsNotBoundToLinux() {}

    /**
     * Date: 2022.12.09
     * Result: All repositories related to this project are version controlled using Git.
     *
     * \SRS  ETB-CFG-10
     */
    auto test_UseGitForVersionControl() {}

    /**
     * Date: 2022.12.09
     * Result: STBSW source code is contained in ESA’s SpaceCreator repository
     * (https://gitrepos.estec.esa.int/taste/spacecreator)
     *
     * \SRS  ETB-CFG-30
     */
    auto test_SedsTasteBridgeSourceIsOnEsaTasteRepository() {}

    /**
     * Date: 2022.12.09
     * Result: SEDS TASTE Bridge SUM is available on the TASTE Wiki
     * https://taste.tuxfamily.org/wiki/index.php?title=Technical_topic:_SedsConverter
     *
     * \SRS  ETB-CFG-50
     */
    auto test_SedsTasteBridgeSumPresentOnTasteWiki() {}

    /**
     * Date: 2022.12.09
     * Result: Schema used for SEDS validation is sourced from the SANA registry at https://sanaregistry.org/r/sois/.
     * The used SEDS schema was downloaded from SANA registry on the 22nd of January 2021.
     *
     * \SRS  ETB-DAT-10
     */
    auto test_SourceSedsValidationSchemaFromSanaRegistry() {}
};

} // namespace seds::test
