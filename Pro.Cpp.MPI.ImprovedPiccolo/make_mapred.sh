#!/usr/bin/env bash

PROGRAM="MapredPagerank.scala"

if [[ "${SPARK}" == "remote" ]]; then
	FLAGS="--master spark://nat-master:7077"
else
	FLAGS=""
fi

function spark_compile {

	set -e

	if [[ "$#" == "0" ]]; then

		echo "Usage: $0 [-e] <scala-file>"
	else

		[[ "$1" == "-e" ]] && EXECUTION="True" && shift

		LOCATION=/tmp/sparkc-${RANDOM}-${RANDOM}-${RANDOM}-${RANDOM}
		BASEPATH="$(dirname $1)"
		BASENAME="$(basename $1 .scala)"

		SOURCEPATH=$(pwd)
	
		rm -rf ${LOCATION} && mkdir -p ${LOCATION}/META\-INF
	
		PACKAGE=$(cat "$1" | grep package | head -n 1 | awk '{ if ($1 == "package") {print $2} }')
	
		if [[ "${PACKAGE}" == "" ]]; then
			MAINCLASS=${BASENAME}
		else
			MAINCLASS=${PACKAGE}.${BASENAME}
		fi
	
		echo "Main-Class: ${MAINCLASS}" > ${LOCATION}/META\-INF/MANIFEST.MF
		if spark-class scala.tools.nsc.Main -bootclasspath $(echo "${BASEPATH}"/lib/*.jar | tr ' ' ':') -d ${LOCATION} $1 ; then
			cd ${LOCATION}
			zip -r ${SOURCEPATH}/${MAINCLASS}.jar ./* >/dev/null
			cd ${SOURCEPATH}
	
			rm -rf ${LOCATION}
			#echo "Run: ./make_pregel.sh [-run-single|-run-cluster]"
			#echo "spark-submit --class ${MAINCLASS} ${SPARK} ./${MAINCLASS}.jar"
			#[[ "${EXECUTION}" != "" ]] && shift && 
			clear && spark-submit ${FLAGS} --class ${MAINCLASS} ./${MAINCLASS}.jar || true
			#rm -f ./${MAINCLASS}.jar
			exit 0
		else
			rm -rf ${LOCATION}
			exit 1
		fi
	fi

}

spark_compile -e ${PROGRAM}

