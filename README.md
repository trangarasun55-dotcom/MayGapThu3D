Hướng dẫn cài đặt và chạy chương trình
1. Yêu cầu công cụ và Môi trường
   - IDE: Mở và biên dịch dự án tốt nhất trên Visual Studio 2026 (hoặc các phiên bản VS hỗ trợ C++ tương đương).
   - Thư viện OpenGL/GLUT: Thư viện GLUT đã được nhóm tích hợp sẵn cục bộ vào trong thư mục glut/ của project (bao gồm các file .h, .lib, .dll).
   Project đã được thiết lập sẵn đường dẫn tương đối để nhận diện thư viện mà không cần cài đặt phức tạp ở môi trường ngoài.

(Nếu máy báo lỗi thiếu thư viện khi chạy trên thiết bị khác, hãy kiểm tra lại cấu hình **C/C++** trong **Properties**: **Additional Include directories** trỏ vào thư mục **glut**, **Linker Input** thêm **glut32.lib**, và **Environment** thêm **path=%path%;$(ProjectDir)\glut**).

3. Cách mở và chạy Project
   - Tải toàn bộ mã nguồn về máy. Đảm bảo giữ nguyên cấu trúc phân cấp thư mục chứa mã nguồn cùng hai thư mục data/ (chứa texture) và glut/.
   - Mở tệp tin **MayGapThu3D_Nhom15_23CN1.sln** bằng Visual Studio.
   - Cấu hình project C/C++ trên VS: (thư viện ngoài)
        + Các file .h/.hpp: Project properties -> C/C++ -> General -> Additional Include directoties -> điền thư mục glut
        + Các file .lib:Project properties -> Linker -> Input -> Điền đường dẫn từng file **glut32.lib**
        + Các file .dll:	Project properties -> Debugging -> Environment -> Điền cú pháp: path=%path%;<thư mục file dll>
   - Chuyển chế độ build sang Debug (hoặc Release) trên nền tảng **x86** (do thư viện GLUT là 32-bit).
   - Nhấn F5 hoặc click nút Local Windows Debugger để biên dịch và chạy chương trình.
     
4. Hướng dẫn thao tác và điều khiển (Controls)
   Chương trình hỗ trợ tương tác qua bàn phím và chuột để điều khiển trực tiếp máy gắp thú cũng như thay đổi góc nhìn camera linh hoạt không gian 3D.
   - Điều khiển càng gắp:
     + Phím Mũi tên Lên (Up): Di chuyển càng gắp lùi ra sau.
     + Phím Mũi tên Xuống (Down): Di chuyển càng gắp tiến tới trước.
     + Phím Mũi tên Trái (Left): Di chuyển càng gắp sang bên trái.
     + Phím Mũi tên Phải (Right): Di chuyển càng gắp sang bên phải.
     + Phím Space (Khoảng trắng): Kích hoạt thả càng gắp xuống đáy để bắt đầu chu trình gắp gấu (chỉ có thể bấm khi máy đang ở trạng thái chờ).
  - Điều khiển Góc nhìn Camera:
     + Phím C (hoặc c): Chuyển đổi qua lại giữa 2 chế độ camera: Góc nhìn toàn cảnh (Panorama) và Góc nhìn cận cảnh (Closeup) bám sát theo vị trí của càng gắp.
     + Kéo thả chuột trái: Nhấn giữ và kéo chuột trái để xoay góc nhìn (pan/tilt) camera tự do xung quanh máy gắp thú.
     + Phím 1: Chuyển nhanh camera về góc nhìn chính diện.
     + Phím 2: Chuyển nhanh camera về góc nhìn bên phải.
     + Phím 3: Chuyển nhanh camera về góc nhìn phía sau.
     + Phím 4: Chuyển nhanh camera về góc nhìn bên trái.
     + Phím 5: Chuyển nhanh camera về góc nhìn chiếu từ trên cao xuống.
  - Các phím chức năng khác:
     + Phím R (hoặc r): Reset lại toàn bộ hệ thống vật lý, tự động sắp xếp lại vị trí gấu bông ngẫu nhiên và đưa càng gắp về điểm xuất phát.
