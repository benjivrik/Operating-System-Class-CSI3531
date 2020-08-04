#  Author : Benjamn Kataliko Viranga
#
#  Class : CSI3531 - Devoir 4
#  
#  FIFO algorithm and LRU algortihm

import random
from queue import Queue

#this function executes the FIFO algorithm page replacement
def fifo(string_ref, num_page_frames):
    
    # initialize your page frame as a list
    # this set will show if the page is already available in one of the FRAME
    frames_data = list()

    # page fault
    fault = 0

    # initialize a Queue for all the elements
    pages_queue = Queue()
    # display frame titles
    title = ""
    for i in range(num_page_frames):
        title += "Frame {}\t".format(i+1)

    # print title
    print(title)
    for page in string_ref: 
        # add element in the frame
        if(len(frames_data) < num_page_frames):
            # frames data
            if( not page in  frames_data ) :
                # element added in the frames data list
                frames_data.append(page)
                # add page in the queue
                pages_queue.put(page)
                # increment the page faults
                # incremented when adding the elements in the frames
                fault += 1

        else : # full frames data - page fault mainly occuring in this section
            if( not page in  frames_data ) :
                # first in first out
                # remove first element of the Queue
                elem = pages_queue.get()
                # get index of the element to be removed in the list
                elem_index = frames_data.index(elem)
                # remove it from the frames_data
                frames_data.remove(elem)
                # add the new page in the queue
                pages_queue.put(page)
                # add the page in the frames data list at the index of the previous element
                frames_data.insert(elem_index,page)
                # increment the page fault
                fault += 1

        # display frame data for each incoming page
        print(display_frame_data(frames_data,page))

    return fault

# this function executes the LRU algorithm page replacement
# remplace la page dont la dernière référence est la plus lointaine
def lru(string_ref, num_page_frames):
    # initialize your page frame as a list
    # this set will show if the page is already available in one of the FRAME
    frames_data = list()

    # page fault
    fault = 0

    # initialize a list for the elements 
    least_ref_pages = list()

    # display frame titles
    title = ""
    for i in range(num_page_frames):
        title += "Frame {}\t".format(i+1)

    # print title
    print(title)

    # execute the algortihm
    for page in string_ref :
         # add element in the frame
        if(len(frames_data) < num_page_frames):
            # frames data
            if( not page in  frames_data ) :
                # element added in the frames data list
                frames_data.append(page)
                # add page in least_ref_pages list
                least_ref_pages.append(page)
                # increment the page faults
                # incremented when adding the elements in the frames
                fault += 1

            else :
                # no page fault occuring here
                # update the least Recenlty Used List since the element is already
                # available in the frames
                # the page triggering this section should already be inside the least_ref_pages 

                if(page in least_ref_pages) : # making sure the page is inside the list

                    # remove the page wherever it is and move it to the last position
                    least_ref_pages.remove(page)
                    # the last element is the page recently used 
                    # the first element is the page least recently used
                    least_ref_pages.append(page)
            
        else : # full frames data 
            if( not page in  frames_data ) : # page fault mainly occuring in this section
                # get and remove the first element of the LRU list
                least_used = least_ref_pages.pop(0)
                # add the new page at the end of LRU List
                least_ref_pages.append(page)
                # find the index of the least_used page in the the frames list
                elem_index = frames_data.index(least_used)
                # remove the least used item from the frames list
                frames_data.remove(least_used)
                # insert the new page at the position of the element removed
                frames_data.insert(elem_index, page)
                # increment the page fault
                fault += 1
                
            else :
                # no page fault occuring here
                # update the least Recenlty Used List since the element is already
                # available in the frames
                # the page triggering this section should already be inside the least_ref_pages 

                if(page in least_ref_pages) : # making sure the page is inside the list

                    # remove the page wherever it is and move it to the last position
                    least_ref_pages.remove(page)
                    # the last element is the page recently used 
                    # the first element is the page least recently used
                    least_ref_pages.append(page)

            
        # display frame data for each incoming page
        print(display_frame_data(frames_data,page))
        # print("**** Fault {}".format(fault)) < DEBUG PURPOSE

    return fault




# display frame data on a row
def display_frame_data(frames_data,page):
    display = ""
    for x in frames_data :
        display += str(x) + "\t"
    display += " < page : "+ str(page)
    return display

# generate a string reference 
def generate_string_ref(num_elements):
    # num_elements is the number of elements you want for the string reference
    string_ref = []
    for elem in range(num_elements):
        string_ref.append(random.randint(0,9))
    
    return string_ref



if __name__ == "__main__" :
    # initialize random set of string
    print("\n>>>>>>>>>>>>>> Starting with FIFO, LRU Following <<<<<<<<<<<<<\n")

    string_ref_size =int(input("Please enter an integer for the size of your string reference :"))
    string_ref = generate_string_ref(string_ref_size)
    num_page_frames = int(input("Please enter an integer for your number of frames :"))
    if(num_page_frames > 7 or num_page_frames < 1):
        print("Max value is 7 and min value is 1")
        while(num_page_frames > 7 or num_page_frames < 1):
            num_page_frames = int(input("Please enter a correct integer for your number of frames :"))
    
    print("\n")
    
    print("> FIFO with string reference {}".format(string_ref))
    result = fifo(string_ref, num_page_frames)
    print("> FIFO page fault result : {}".format(result))
    
    print("\n")

    print("> LRU with string reference {}".format(string_ref))
    result = lru(string_ref, num_page_frames)
    # result = lru([2,3,2,1,5,2,4,5,3,2,5,2],3) < example in class module 8 - p28 | 7 page faults for LRU
    print("> LRU page fault result : {}".format(result))
