#!/bin/bash


phone_book="file.txt"
add_new_contact(){
    echo "enter a contact name"
    read contact
    echo "enter a content number"
    read number
    echo "$contact:$number" >> "$phone_book"
    echo "contacts were added"
}

view_all_contacts(){
    echo "all contacts"
    cat "$phonebook"
}

search_by_name(){
    echo "enter name"
    read name 
    grep "$name" "$phone_book" || echo "contacts not found"
}

delete_contact(){
    echo "enter a contact name to delete"
    read name 
    echo "are you sure n/y "
    read choice 
    if [ $choice -eq 'y' ]; then
        sed -i "/$name/d" "$phone_book"
    else return 
    fi
}

while true; do
    echo "1) add new contact"
    echo "2) view all contacts"
    echo "3) search by name"
    echo "4) delete contacts"
    echo "5) exit"
    read  choice 

    case $choice in 
       1) add_new_contact;;


    esac
end 