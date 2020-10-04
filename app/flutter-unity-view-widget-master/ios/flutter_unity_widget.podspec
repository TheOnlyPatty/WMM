#
# To learn more about a Podspec see http://guides.cocoapods.org/syntax/podspec.html
#
Pod::Spec.new do |s|
  s.name             = 'flutter_unity_widget'
  s.version          = '0.0.1'
  s.summary          = 'Flutter unity 3D widget for embedding unity in flutter'
  s.description      = <<-DESC
Flutter unity 3D widget for embedding unity in flutter
                       DESC
  s.homepage         = 'http://xraph.com'
  s.license          = { :file => '../LICENSE' }
  s.author           = { 'Rex Isaac Raphael' => 'rex.raphael@outlook.com' }
  s.source           = { :path => '.' }
  s.source_files = 'Classes/**/*'
  s.public_header_files = 'Classes/**/*.h'
  s.dependency 'Flutter'
  s.frameworks = 'UnityFramework'

  s.ios.deployment_target = '8.0'
  s.xcconfig = {
     'FRAMEWORK_SEARCH_PATHS' => '"${PODS_ROOT}/../UnityLibrary" "${PODS_ROOT}/../.symlinks/flutter/ios-release" "${PODS_CONFIGURATION_BUILD_DIR}"', 
     'OTHER_LDFLAGS' => '$(inherited) -framework UnityFramework ${PODS_LIBRARIES}'
   }
end
