.section .rodata
.global driver_splash_image

splash_image:
.incbin "../../asset/splash.gif"

driver_splash_image:
.word splash_image