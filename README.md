Minimal OpenVR demo code to show issue with calling `SetActionManifestPath` under Wine that was discovered in as
part of the Revive [Linux issue](https://github.com/LibreVR/Revive/issues/1571).

# Issue

Calling
```C++
vr::VRInput()->SetActionManifestPath("...absolute path to input manifest json...");
```
fails with return code `4` (`VRInputError_InvalidParam`) and the associate proton log file shows
```
016c:trace:vrclient:winIVRInput_IVRInput_010_SetActionManifestPath 0x2fa840
Fri Nov 17 2023 05:01:40.668447 [Error] - [Input] SetActionManifestPath requires an absolute path. 'à§/' is not absolute.
```
So it seems the path is getting lost or interpreted incorrectly somewhere along the way.

# Building

Should be buildable on Windows using either clang or msvc. Should be able to generate a Visual Studio file using
something like
```
cmake -G "Visual Studio VV YYYY" -A x64 -B build
```
where you replace VV with version and YYYY with version year (see [cmake visual studio
generators](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html#id14)). I have not personally
tried this though as I don't have Windows.

It is buildable for sure under Linux using [msvc-wine](https://github.com/mstorsjo/msvc-wine)
```
CC=cl CXX=cl cmake -B build64 \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_SYSTEM_NAME=Windows \
  -DCMAKE_VS_PLATFORM_NAME=x64 \
  -DCMAKE_INSTALL_PREFIX=(pwd)
cd build64
make install
```
Should be fine with mingw64 and clang too (I haven't bothered trying these as I was already using msvc-wine
for building Revive).

# Logs

A typical run of the program looks like this
```
C:\OpenVR64>test C:\Revive\Input\action_manifest.json
```
```
Sleeping for 20s to allow tracing or debugging attachment...
success: vr::VR_Init(&err, vr::VRApplication_Scene) = 0
failure: vr::VRInput()->SetActionManifestPath("C:\Revive\Input\action_manifest.json") = 4
```
The steam proton logs complain `SetActionManifestPath requires an absolute path. 'ð¶/' is not absolute.`, so
the path is definitely getting munged somehow along the way.
```
01c4:trace:vrclient:DllMain (0x7f1fb1970000, 1, (nil))
01c4:trace:vrclient:VRClientCoreFactory name: IVRClientCore_003, return_code: 0x12fd50
01c4:trace:vrclient:load_vrclient got openvr runtime path: /home/tyson/.local/share/Steam/steamapps/common/SteamVR/bin/linux64/vrclient.so
01c4:trace:vrclient:create_win_interface trying to create IVRClientCore_003
01c4:trace:vrclient:create_winIVRClientCore_IVRClientCore_003 -> 0x2fb630
01c4:trace:vrclient:winIVRClientCore_IVRClientCore_003_Init 0x2fb630
Fri Nov 17 2023 05:53:49.813062 [Info] - //==============================================================================================
Fri Nov 17 2023 05:53:49.813477 [Info] - ================================================================================================
Fri Nov 17 2023 05:53:49.813568 [Info] - ================================================================================================
Fri Nov 17 2023 05:53:49.813750 [Info] - wine64-preloader 2.0.10 startup with PID=1836153, config=/home/tyson/.local/share/Steam/config, runtime=/home/tyson/.local/share/Steam/steamapps/common/SteamVR
Fri Nov 17 2023 05:53:49.814062 [Info] - vrclient type=VRApplication_Scene
Fri Nov 17 2023 05:53:49.814265 [Info] - Skipping HMD presence checks.
Fri Nov 17 2023 05:53:49.834694 [Info] - [Settings] Load Default Json Settings from /home/tyson/.local/share/Steam/steamapps/common/SteamVR/drivers/htc/resources/settings/default.vrsettings
Fri Nov 17 2023 05:53:49.835485 [Info] - [Settings] Load Default Json Settings from /home/tyson/.local/share/Steam/steamapps/common/SteamVR/drivers/lighthouse/resources/settings/default.vrsettings
Fri Nov 17 2023 05:53:49.836118 [Info] - [Settings] Load Default Json Settings from /home/tyson/.local/share/Steam/steamapps/common/SteamVR/drivers/null/resources/settings/default.vrsettings
Fri Nov 17 2023 05:53:49.838452 [Info] - [Settings] Load Default Json Settings from /home/tyson/.local/share/Steam/steamapps/common/SteamVR/resources/settings/default.vrsettings
Fri Nov 17 2023 05:53:49.839265 [Info] - [Settings] Load Json Settings from /home/tyson/.local/share/Steam/config/steamvr.vrsettings
Fri Nov 17 2023 05:53:49.841161 [Info] - CIPCPipe::ConnectPipe(SteamVR_Namespace) attempting connect to /steamvr/SteamVR_Namespace
Fri Nov 17 2023 05:53:49.841711 [Info] - CSharedResourceNamespaceClient::Init(): received namespace data 1813362
Fri Nov 17 2023 05:53:49.841949 [Info] - CIPCPipe::ConnectPipe(VR_ServerPipe_1813362) attempting connect to /steamvr/VR_ServerPipe_1813362
Fri Nov 17 2023 05:53:52.369032 [Info] - Received success response from vrserver connect
Fri Nov 17 2023 05:53:54.996504 [Info] - Timed out waiting for a good app key
Fri Nov 17 2023 05:53:54.996830 [Info] - App key after connect message:steam.app.0
Fri Nov 17 2023 05:53:57.401040 [Info] - CIPCPipe::ConnectPipe(VR_CompositorPipe_1813362) attempting connect to /steamvr/VR_CompositorPipe_1813362
Fri Nov 17 2023 05:53:57.402505 [Info] - Received success response from vrcompositor connect
Fri Nov 17 2023 05:53:57.403600 [Info] - Initializing the limited version of CVRCompositorClient
Fri Nov 17 2023 05:53:58.178915 [Info] - [Input] Attempting to load initial input config
Fri Nov 17 2023 05:53:58.656936 [Info] - Determined this is a legacy app.
Fri Nov 17 2023 05:53:58.790840 [Info] - Determined this is a legacy app.
Fri Nov 17 2023 05:53:58.882134 [Info] - Unable to find input 'z' in filter click_button_actions_legacy_17_user_hand_left_input_thumbstick
Fri Nov 17 2023 05:53:58.882489 [Info] - Unable to find input 'z' in filter click_button_actions_legacy_18_user_hand_right_input_thumbstick
Fri Nov 17 2023 05:53:58.895489 [Info] - [Input] After initial config load, app appears to be using legacy input
Fri Nov 17 2023 05:53:58.895700 [Info] - /user/hand/left is 1
Fri Nov 17 2023 05:53:58.895813 [Info] - /user/hand/right is 2
Fri Nov 17 2023 05:53:58.898129 [Info] - TrackedControllerRole_LeftHand (1)-> 
Fri Nov 17 2023 05:53:58.898468 [Info] - TrackedControllerRole_RightHand (2)-> 
Fri Nov 17 2023 05:53:58.898588 [Info] - TrackedControllerRole_Invalid (0)-> 
Fri Nov 17 2023 05:53:59.862538 [Info] - VR Dashboard executable already running.
01c4:trace:vrclient:winIVRClientCore_IVRClientCore_003_IsInterfaceVersionValid 0x2fb630
01c4:trace:vrclient:winIVRClientCore_IVRClientCore_003_GetGenericInterface 0x2fb630
01c4:trace:vrclient:ivrclientcore_get_generic_interface 0x7f1fb14eebc8 0x14000aef0
01c4:trace:vrclient:create_win_interface trying to create IVRSystem_022
01c4:trace:vrclient:create_winIVRSystem_IVRSystem_022 -> 0x2fb6f0
01c4:trace:vrclient:winIVRClientCore_IVRClientCore_003_GetGenericInterface 0x2fb630
01c4:trace:vrclient:ivrclientcore_get_generic_interface 0x7f1fb14f26e8 0x14000ac78
01c4:trace:vrclient:create_win_interface trying to create IVRInput_010
01c4:trace:vrclient:create_winIVRInput_IVRInput_010 -> 0x2fb750
01c4:trace:vrclient:winIVRInput_IVRInput_010_SetActionManifestPath 0x2fb750
Fri Nov 17 2023 05:54:01.930713 [Error] - [Input] SetActionManifestPath requires an absolute path. 'ð¶/' is not absolute.
01c4:fixme:kernelbase:AppPolicyGetProcessTerminationMethod FFFFFFFFFFFFFFFA, 000000000012FCB0
01c4:trace:vrclient:DllMain (0x7f1fb1970000, 0, 0x1)
...
```

# Other Details

I also discovered that your SteamVR version needs to match you OpenVR version of the `VR_Init` call will fail.
This means the (much better performaing for me at least) linux_v1.14 SteamVR beta will not work with programs
compiled against the newest OpenVR releases. This includes this, although, for no good reason. Feel free to
revert the openvr submodule to an earlier version.

As a full disclaimer, I have also been testing under
[GE-Proton8-23](https://github.com/GloriousEggroll/proton-ge-custom/releases) as I needed this for other reasons
when testing Revive and Linux. It is not a requirement for this program, and I am assuming the bug it demonstrates
also occurs with the stock Steam Proton.

