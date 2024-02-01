# Food Reservation System
This project is a console-based Food Reservation System implemented in C. The system allows users to log in, sign up, and perform various operations related to food reservations. Users can interact with the system by either entering commands directly or providing a file with a sequence of commands.

## Features

* Login/Logout: Users can log in with their credentials, and the system distinguishes between regular users and administrators.

* Sign Up: New users can sign up by providing their personal information. The system checks for duplicate user IDs and ensures valid input.

* User Menu:

  * Logout: Allow users to log out of the system.
  * Change Password: Enable users to change their password.
  * Change Date And Time: Allow users to update their date and time preferences.
  * Reserve Food: Users can reserve food for a specific date.
  * Check News: Users can check the latest news.
  * Charge Account: Users can add funds to their account.
  * Send Charge: Users can send charges to others.
  * Vote: Users can participate in polls.

* Personnel/Admin Menu:
  * Logout: Allow personnel/admin to log out of the system.
  * Change Password: Enable personnel/admin to change their password.
  * Approve User's Register: Personnel can approve new user registrations.
  * Change Student's Password: Personnel can change the passwords of students.
  * Remove Student: Personnel can remove student accounts.
  * Deactivate Student: Personnel can deactivate student accounts.
  * Define Self: Personnel can define their own information.
  * Define Food: Personnel can define food items.
  * Define Meal Plan: Personnel can define meal plans.
  * Charge Student Account: Personnel can add funds to students' accounts.
  * Change Date And Time: Allow personnel/admin to update system date and time.
  * Add News: Personnel/admin can add news for users.
  * Add Poll: Personnel/admin can add polls for user voting.

* File Input: The system reads input from a file, allowing batch processing of commands. Input files can contain commands such as register, login, logout, reserve-food, define-food, approve, and more.

## Usage

### Compile: 

Compile the program using a C compiler, such as GCC.

```bash
gcc project.c -o food_reservation_system
```

### Run: 

Run the compiled executable.

```bash
./food_reservation_system
```

### Choose Input Method: 

Select whether to input commands from a file or enter them directly.

### Interact: 

If entering commands directly, follow the on-screen prompts. If using a file, provide the file address when prompted.

### Input File Format

The input file should contain lines with commands in the following format:

```plaintext
# command:param1|param2|param3:...
```

#### Supported commands include register, login, logout, reserve-food, define-food, approve, and more. Refer to the code for a complete list of supported commands.

### Sample Input File

```plaintext
# register:name|John|family|Doe|gender|male|type|user|birthdate|2000-01-15|national-id-code|1234567890|user-id|john123|password|pass123
# login:user|john123|password|pass123
# reserve-food:user|john123|food-type|lunch|reservation-date|2024-02-10
# logout:user|john123
```

## Notes
- The system uses a binary file (user.bin) to store user information.
- The code includes error handling for invalid input and duplicate user IDs.
- The address_maker function dynamically creates input and output file addresses.
- Feel free to customize and expand this project based on your specific requirements for a food reservation system!
