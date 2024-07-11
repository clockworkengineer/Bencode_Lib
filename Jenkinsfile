pipeline {
    agent any
    stages {
        stage ('Build') {
            steps {
                sh 'echo "Building..."'
                sh 'chmod +x ./scripts/Linux-Build.sh'
                sh './scripts/Linux-Build.sh'
                archiveArtifacts artifacts: 'build/tests/Bencode_Lib_Unit_Tests', fingerprint: true
            }
        }
        stage ('Test'){
            steps {
                sh 'echo "Testing..."'
                sh 'chmod +x ./scripts/Linux-Run.sh'
                sh './scripts/Linux-Run.sh'
            }
        }
    }
}