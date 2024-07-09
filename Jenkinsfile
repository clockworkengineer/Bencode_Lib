pipeline {
    agent any
    stages {
        stage ('Build') {
            steps {
                sh 'echo "Building..."'
                sh 'chmod -x scripts/Linx-Build.sh'
                sh 'scripts/Linux-Build.sh'
            }

        }
    }
}