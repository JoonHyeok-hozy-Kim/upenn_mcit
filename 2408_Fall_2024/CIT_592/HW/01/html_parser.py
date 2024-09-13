import os
from html.parser import HTMLParser

class CourseHTMLParser(HTMLParser):
    class MyNode:
        def __init__(self, parent, tag) -> None:
            self._parent = parent
            self._children = []
            self._tag = tag
            self._attributes = None
            self._data = None
        
        def add_data(self, data):
            self._data = data
        
        def add_attributes(self, attrs):
            self._attributes = attrs
        
        def add_child(self, child):
            self._children.append(child)

    class MyTree:    
        class MyClass:
            def __init__(self) -> None:
                self._code = None
                self._instructor = None
            
            def set_code(self, code):
                self._code = code
            
            def set_instructor(self, instructor):
                self._instructor = instructor

            def __str__(self) -> str:
                result = []
                result.append('') if self._code is None else result.append(self._code)
                result.append(' / ')
                result.append('') if self._instructor is None else result.append(self._instructor)
                return ''.join(result)

        def __init__(self, root) -> None:
            self._root = root
            self._course_and_instructor = []
        
        def pre_order(self, curr_node=None, cnt=0, class_obj=None):
            if curr_node is None:
                curr_node = self._root

            curr_class = class_obj
            if curr_node._attributes is not None:
                for att in curr_node._attributes:
                    if att[0] == 'class' and att[1] == 'result result--group-start':
                        curr_class = self.MyClass()
                    elif curr_node._tag == 'a' and att[0] == 'data-group' and curr_class is not None:
                        course_code = att[1].split(':')[1]
                        dept = course_code.split(' ')[0]
                        num = course_code.split(' ')[1]
                        # curr_class.set_code(course_code)
                        if dept in ('CIT', 'CIS') and 6000 >= int(num) >= 5000:
                            curr_class.set_code(course_code)
                    elif curr_node._tag == 'span' and att[0] == 'class' and att[1] == 'result__flex--9 text--right':
                        curr_class.set_instructor(curr_node._data)

            for c in curr_node._children:
                self.pre_order(c, cnt+1, curr_class)
            
            if curr_node._tag == 'div' and curr_class is not None and curr_class._code is not None:
                self._course_and_instructor.append(curr_class)
        
        def get_courses_and_instructors(self):
            return self._course_and_instructor

    def __init__(self, *, convert_charrefs: bool = True) -> None:
        super().__init__(convert_charrefs=convert_charrefs)
        self._root = self.MyNode(None, None)
        self._tree = self.MyTree(self._root)
        self._curr = self._root

    def handle_starttag(self, tag, attrs):
        new_node = self.MyNode(self._curr, tag)
        new_node.add_attributes(attrs)
        self._curr.add_child(new_node)
        self._curr = new_node

    def handle_endtag(self, tag):
        self._curr = self._curr._parent

    def handle_data(self, data):
        self._curr.add_data(data)

class InstructorHTMLParser(HTMLParser):
    class MyNode:
        def __init__(self, parent, tag) -> None:
            self._parent = parent
            self._children = []
            self._tag = tag
            self._attributes = None
            self._data = None
        
        def add_data(self, data):
            self._data = data
        
        def add_attributes(self, attrs):
            self._attributes = attrs
        
        def add_child(self, child):
            self._children.append(child)

    class MyTree:    
        def __init__(self, root) -> None:
            self._root = root
            self._instructors = []
        
        def pre_order(self, curr_node=None):
            if curr_node is None:
                curr_node = self._root

            for c in curr_node._children:
                self.pre_order(c)
        
        def get_courses_and_instructors(self):
            return self._instructors

    def __init__(self, *, convert_charrefs: bool = True) -> None:
        super().__init__(convert_charrefs=convert_charrefs)
        self._root = self.MyNode(None, None)
        self._tree = self.MyTree(self._root)
        self._curr = self._root

    def handle_starttag(self, tag, attrs):
        new_node = self.MyNode(self._curr, tag)
        new_node.add_attributes(attrs)
        self._curr.add_child(new_node)
        self._curr = new_node

    def handle_endtag(self, tag):
        self._curr = self._curr._parent

    def handle_data(self, data):
        self._curr.add_data(data)



if __name__ == '__main__':
    courses_list = []
    course_dict = {}

    os.chdir(r'C:\Users\danie\git_hub\upenn_mcit\2408_Fall_2024\CIT_592\HW')
    html_files = ['cit.html', 'cis.html']

    for html in html_files:
        with open(html, 'r') as file:
            data = file.read().replace('\n', '')

        parser = CourseHTMLParser()
        parser.feed(data)
        parser._tree.pre_order()
        courses_list.extend(parser._tree.get_courses_and_instructors())
        for course in parser._tree.get_courses_and_instructors():
            if course._code in course_dict and course_dict[course._code] is not None:
                # print(course._code, course._instructor, course_dict[course._code])
                continue
            course_dict[course._code] = course._instructor

    # for c in courses_list:
    #     print(c)

    for k in course_dict.keys():
        print(k, course_dict[k])



    
    with open('instructor.html', 'r') as file:
        data = file.read().replace('\n', '')

    parser = InstructorHTMLParser()
    parser.feed(data)