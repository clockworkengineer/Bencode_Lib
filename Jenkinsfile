pipeline {
    agent any
    stages {
        stage ('Build') {
            steps {
                sh 'echo "Building..."'
                sh 'chmod -x ./scripts/Linx-Build.sh'
                sh './scripts/Linux-Build.sh'
                archiveArtifacts artifacts: 'build/tests/Bencode_Lib_Unit_Tests', fingerprint: true
            }
        }
        stage ('Run'){
            steps {
                sh 'echo "Running..."'
            }
        }
    }
}