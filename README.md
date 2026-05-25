# MayGapThu3D
MayGapThu3D_Nhom15_23CN1/    # Thư mục gốc
├── Debug/
│
├── glut/                        # GIỮ NGUYÊN HOÀN TOÀN CỦA THẦY
│   ├── glut.dll
│   ├── glut.h
│   ├── glut.lib
│   ├── glut32.dll
│   └── glut32.lib
│
├── data/                        # Giữ nguyên để chứa ảnh texture của Huyền
│   ├── bricks.bmp
│   ├── sky.bmp
│   └── ... (các file ảnh .bmp dùng cho máy gắp thú)
│
├── imageloader.cpp              # để làm nền tảng đọc ảnh
├── imageloader.h
│
├── main.cpp                     # File chạy chính của nhóm 15 (Đổi từ main.cpp của thầy)
├── camera.cpp / .h                # [Trang] Xử lý góc nhìn camera, phím C
├── input.cpp / .h                 # [Trang] Xử lý sự kiện bàn phím (Mũi tên, Space, R)
├── render.cpp / .h                # [Huyền] Vẽ thân máy, cần gắp, gấu bông, ánh sáng
└── physics.cpp / .h               # [Nhung] Xử lý va chạm AABB, logic nhấc vật thể
