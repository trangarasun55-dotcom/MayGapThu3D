# MayGapThu3D
MayGapThu3D_Nhom15_23CN1/    # Thư mục gốc
├── .vs/
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
├── imageloader.c                # Đổi đuôi .cpp của thầy sang .c để làm nền tảng đọc ảnh
├── imageloader.h
│
├── main.c                       # File chạy chính của nhóm 15 (Đổi từ main.cpp của thầy)
├── camera.c / .h                # [Trang] Xử lý góc nhìn camera, phím C
├── input.c / .h                 # [Trang] Xử lý sự kiện bàn phím (Mũi tên, Space, R)
├── render.c / .h                # [Huyền] Vẽ thân máy, cần gắp, gấu bông, ánh sáng
└── physics.c / .h               # [Nhung] Xử lý va chạm AABB, logic nhấc vật thể
