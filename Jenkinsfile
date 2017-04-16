stage('Test') {
    parallel linux: {
        node('linux') {
            checkout scm
            sh 'python test_runner.py'
        }
    },
    mac: {
        node('mac') {
            checkout scm
            sh 'python test_runner.py'
        }
    }
}