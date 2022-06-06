import cv2
import numpy as np
import stapipy as st
import time

number_of_images_to_grab = 1

DISPLAY_RESIZE_FACTOR = 0.3

def cam_init():

    st.initialize()

    st_system = st.create_system()
    st_device = st_system.create_first_device()
    #print('Device=', st_device.info.display_name)

    st_datastream = st_device.create_datastream()
    

    return st_datastream, st_device


def camRead(st_datastream, st_device):

    st_datastream.start_acquisition(number_of_images_to_grab)
    st_device.acquisition_start()
    
    while st_datastream.is_grabbing:
        with st_datastream.retrieve_buffer() as st_buffer:
            if st_buffer.info.is_image_present:
                st_image = st_buffer.get_image()
                """
                print("BlockID={0} Size={1} x {2} First Byte={3}".format(
                        st_buffer.info.frame_id,
                        st_image.width, st_image.height,
                        st_image.get_image_data()[0]))
                """
                pixel_format = st_image.pixel_format
                pixel_format_info = st.get_pixel_format_info(pixel_format)


                if not(pixel_format_info.is_mono or pixel_format_info.is_bayer):
                    continue

                data = st_image.get_image_data()
                if pixel_format_info.each_component_total_bit_count > 8:
                    nparr = np.frombuffer(data, np.uint16)
                    division = pow(2, pixel_format_info
                                   .each_component_valid_bit_count - 8)
                    nparr = (nparr / division).astype('uint8')
                else:
                    nparr = np.frombuffer(data, np.uint8)

                nparr = nparr.reshape(st_image.height, st_image.width, 1)

                # Perform color conversion for Bayer.
                if pixel_format_info.is_bayer:
                    bayer_type = pixel_format_info.get_pixel_color_filter()
                    if bayer_type == st.EStPixelColorFilter.BayerRG:
                        nparr = cv2.cvtColor(nparr, cv2.COLOR_BAYER_RG2RGB)
                    elif bayer_type == st.EStPixelColorFilter.BayerGR:
                        nparr = cv2.cvtColor(nparr, cv2.COLOR_BAYER_GR2RGB)
                    elif bayer_type == st.EStPixelColorFilter.BayerGB:
                        nparr = cv2.cvtColor(nparr, cv2.COLOR_BAYER_GB2RGB)
                    elif bayer_type == st.EStPixelColorFilter.BayerBG:
                        nparr = cv2.cvtColor(nparr, cv2.COLOR_BAYER_BG2RGB)

                nparr = cv2.resize(nparr, None,
                                   fx=DISPLAY_RESIZE_FACTOR,
                                   fy=DISPLAY_RESIZE_FACTOR)
                #return nparr
                #cv2.imshow('image', nparr)
                #cv2.waitKey(1)
                st_device.acquisition_stop()
                st_datastream.stop_acquisition()
                return nparr
            else:
                print("Image data does not exist.")

        
    #st_device.acquisition_stop()
    #st_datastream.stop_acquisition()

"""
                
if __name__ == "__main__":
    st_datastream, st_device = cam_init()
    main(st_datastream, st_device)
    time.sleep(3)
    main(st_datastream, st_device)

"""
