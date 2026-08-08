// CI pipeline: build the stack in the pinned container, test it, verify the
// signature, and stage it for deployment. A red stage blocks the release.
pipeline {
  agent { dockerfile true }   // run every stage inside the pinned Dockerfile toolchain
  stages {
    stage('Build') {
      steps {
        sh 'make'                                  // firmware + .ko + dtb (top-level Makefile)
      }
    }
    stage('Test') {
      steps {
        sh 'make -C kernel check'                  // sparse/static checks on the module
        sh 'python3 -m pyflakes monitor/dpu_monitor.py'
      }
    }
    stage('Verify signature') {
      steps {
        sh './secure/verify_image.sh build/Image'  // chain-of-trust gate
      }
    }
    stage('Stage artifact') {
      steps {
        archiveArtifacts artifacts: 'build/*', fingerprint: true
      }
    }
  }
  post {
    failure { echo 'Pipeline failed — release blocked.' }
  }
}
