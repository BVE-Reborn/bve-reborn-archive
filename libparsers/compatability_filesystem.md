# Compatibility Filesystem

This is a short reference of the compatability filesystem. They are objects that are included by default within BVE-Reborn and there needs to be some way to reference them, though they most likely won't even be files in the filesystem.

## Folder Hirearchy

- \034compat\034
  - limit
    - left-side
      - left_bound
      - no_bound
      - right_bound
    - right-side
      - left_bound
      - no_bound
      - right_bound
    - no_restriction
  - signal
    - default
      - 2a
      - 2b
      - 3
      - 4a
      - 4b
      - 5a
      - 5b
      - 6
      - relay
    - user
      - \<signal name\>
        - \034
          - \<glow name\>
  - transponder
    - S
    - SN
    - AccidentalDep
    - ATSP-Pattern
    - ATSP-Immediate
