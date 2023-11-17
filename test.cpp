#include <Windows.h>
#include <openvr.h>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("%s <absolute path of action manifest>", argc ? argv[0] : "test.exe");
    exit(1);
  }   

  printf("Sleeping for 20s to allow tracing or debugging attachment...\n");
  Sleep(20000);
  
  vr::HmdError herr;
  vr::VR_Init(&herr, vr::VRApplication_Scene);
  printf("%s: vr::VR_Init(&err, vr::VRApplication_Scene) = %d\n",
         herr == vr::VRInitError_None ? "success" : "failure", herr);
  if (herr != vr::VRInitError_None) 
    exit(1);

  vr::EVRInputError ierr;
  ierr = vr::VRInput()->SetActionManifestPath(argv[1]);
  printf("%s: vr::VRInput()->SetActionManifestPath(\"%s\") = %d\n",
         ierr == vr::VRInputError_None ? "success" : "failure", argv[1], ierr);
  if (ierr != vr::VRInputError_None)
    exit(1);

  vr::VR_Shutdown();
  return 0;
}
