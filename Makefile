build:
	python3 ./scripts/build_spacecreator.py --project_dir ${HOME}/project/spacecreator/ --app_dir ${HOME}/project/spacecreatorenv6/spacecreator.AppDir --env_dir ${HOME}/project/spacecreatorenv6 --build_dir ${HOME}/project/spacecreator_build --build_type Release --no_build_asn1plugin

postbuild:
	 python3 ./scripts/postbuild.py --build_dir ${HOME}/project/spacecreator_build --app_dir ${HOME}/project/spacecreatorenv6/spacecreator.AppDir

appimage:
	python3 ./scripts/create_appimage.py --output_dir ${HOME}/project/spacecreator_build  --project_dir ${HOME}/project/spacecreator  --app_dir ${HOME}/project/spacecreatorenv6/spacecreator.AppDir --env_dir ${HOME}/project/spacecreatorenv6

copy_to_internal_directory:
	cp -R ${HOME}/project/spacecreator/* ${HOME}/project/metmod


all: build_plg copy_into_build_directory build_app_img 
