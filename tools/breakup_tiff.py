from PIL import Image
import PIL
import numpy as np
import cv2

def displayMap(map):
        map = map-np.min(map)
        normed_image = np.array(255*(map/np.max(map)),dtype=np.uint8)
        im_resized = cv2.resize(normed_image,(512,512))         
        cv2.imshow("window",im_resized)
        cv2.waitKey(0)

PIL.Image.MAX_IMAGE_PIXELS = 2000000000

IMG_X = 3200
IMG_Y = 3200
PIXEL_STRIDE = 1600 # 5m/px * 1600 px = 8Km
# IMG_X = 40000
# IMG_Y = 40000
# PIXEL_STRIDE = 2000 # 5m/px * 2000 px = 10Km

OUTPUT_DIR = ".\\topo_maps\\"

im = Image.open("c:\\Users\\luker\\Downloads\\Site04_final_adj_5mpp_surf.tif")
# im = Image.open("c:\\Users\\luker\\OneDrive - UCB-O365\\Classes\\2023_fall\\ASEN5519\\project\\data\\ldem_87s_5mpp.tiff")
print(im.size)
arr = np.array(im)
print(arr.shape)
# displayMap(arr)

# put into 10km x 10km images

for i in range(0,IMG_X,PIXEL_STRIDE):
    for j in range(0,IMG_Y,PIXEL_STRIDE):
        print(f"saving image from x = [{i}:{i+PIXEL_STRIDE}] to y = [{j}:{j+PIXEL_STRIDE}] ")
        sub_arr = arr[i:i+PIXEL_STRIDE, j:j+PIXEL_STRIDE]
        # name = "\\x{0:03d}y{0:03d}km.tiff".format(int(i/1000),int(j/1000))
        name = "\\x{}y{}km.tiff".format(int(i),int(j))
        # sub_arr.save(OUTPUT_DIR+name)
        sub_im = Image.fromarray(sub_arr)
        sub_im.save(OUTPUT_DIR+name)
        displayMap(sub_arr)

        





# normed_image = np.array(255*arr/np.max(arr),dtype=np.uint8)
# im_resized = cv2.resize(normed_image,(512,512))         
# cv2.imshow("window",im_resized)
# cv2.waitKey(0)